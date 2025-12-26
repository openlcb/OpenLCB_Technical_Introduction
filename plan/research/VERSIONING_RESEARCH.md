# OpenMRNLite Configuration Versioning Research

## Summary: What Actually Happens with Version Mismatches

When the firmware's `CANONICAL_VERSION` differs from the saved config file version:
- **Version mismatch ALWAYS triggers a factory reset** (complete EEPROM wipe except first 128 bytes)
- **NO field mapping or data migration occurs** (there is zero support for this)
- **NO automatic defaults are applied** to new fields (uninitialized space is 0xFF)
- **Data preservation is minimal**: only the first 128 bytes (ACDI user data: node name/description) are preserved
- **All application config data is wiped**, including event IDs

---

## Version Checking Logic

### Location: [SimpleStack.cpp](OpenMRNLite/src/openlcb/SimpleStack.cpp#L276)

The system has two functions:

#### 1. `create_config_file_if_needed()` (Initial creation or recovery)
- **When called**: During stack startup, creates/repairs config file
- **Version check**: `if (!reset && cfg.version().read(fd) != expected_version)`
- **Action on mismatch**: Sets `reset = true`

#### 2. `check_version_and_factory_reset()` (Manual version check)
- **When called**: Can be called explicitly before startup
- **Version check**: `if (cfg.version().read(fd) != expected_version)`
- **Action on mismatch**: Immediately triggers reset

### Exact Code Logic (from `create_config_file_if_needed`):

```cpp
if (!reset && cfg.version().read(fd) != expected_version)
{
    uint16_t current_version = cfg.version().read(fd);
    if (current_version != expected_version)
    {
        LOG(VERBOSE, "config version mismatch (%d vs %d), forcing reset.",
            current_version, expected_version);
        reset = true;  // ← THIS TRIGGERS FACTORY RESET
    }
}
```

---

## What Gets Cleared on Factory Reset

When `reset = true` is triggered:

```cpp
// Clears the file, preserving the node name and desription if any.
if (extend && !reset)
{
    // Only extends with 0xFF (preserve ACDI)
}
else if (statbuf.st_size >= 128)
{
    auto ret = lseek(fd, 128, SEEK_SET);  // ← PRESERVE first 128 bytes (ACDI)
    file_size -= 128;
}
else
{
    lseek(fd, 0, SEEK_SET);  // Erase everything if file < 128 bytes
}

// Fill rest of file with 0xFF
memset(buf, 0xff, bufsize);
while (len > 0)
{
    write(fd, buf, std::min(len, bufsize));
}

// Then initialize critical fields:
cfg.version().write(fd, expected_version);     // Update version
cfg.next_event().write(fd, 0);                 // Reset event counter
Uint8ConfigEntry(0).write(fd, 2);              // ACDI version byte
factory_reset_all_events(cfg, node()->node_id(), fd);  // Generate new event IDs
configUpdateFlow_.factory_reset();             // Call all listeners' factory_reset()
```

**Result**: Bytes 0-127 preserved (ACDI user data like node name), bytes 128+ filled with 0xFF

---

## Field Mapping/Migration Support

**ANSWER: There is NO field mapping or migration mechanism.**

Evidence:
- No code path compares old config schema to new schema
- No selective field copying from old config
- No field-by-field migration logic
- When version mismatch is detected, the entire application config section is wiped

The only preservation is the ACDI user data (first 128 bytes) which contains:
- Node name (set by user via JMRI)
- Node description
- ACDI user version byte

This is preserved because the code explicitly reserves the first 128 bytes before writing 0xFF.

---

## How Defaults Are Applied for New Fields

When firmware adds new fields and version isn't bumped:

### Scenario 1: File Extension (Version Same, File Too Short)
If `file_size < CONFIG_FILE_SIZE`:
```cpp
if (statbuf.st_size < (ssize_t)file_size)
{
    extend = true;  // File will be extended
}
```

Extension fills NEW space with **0xFF (raw uninitialized bytes)**:
```cpp
if (extend && !reset)
{
    lseek(fd, statbuf.st_size, SEEK_SET);  // Seek to end
    file_size -= statbuf.st_size;           // Just extend
    // ... write 0xFF to new space
}
```

**Result**: New fields read as 0xFFFFFFFF (unmasked), not defaults

### Scenario 2: Reading Uninitialized 0xFF Values
When `apply_configuration()` listeners read new fields:
- Config entries read raw bytes: `template<class T> T raw_read(int fd)`
- No default substitution during read
- **The application code receives 0xFF bytes as-is**

However, some components use defensive reading:
```cpp
// Example from ServoConsumer.hxx
const int16_t cfg_servo_min_pct = cfg_.servo_min_percent().read(fd);
```

If the field is at an uninitialized location, this reads 0xFFFF (all bits set).
- **No automatic truncation to declared min/max**
- Application must handle or use `CDI_READ_TRIMMED()` macro

### Scenario 3: Using CDI_READ_TRIMMED (Safe Default Handling)
Some code uses:
```cpp
#define CDI_READ_TRIMMED(PATH, fd) \
    PATH().read_or_write_trimmed(fd, PATH##_options().minvalue(), \
                                      PATH##_options().maxvalue())
```

This reads value and writes default if out of range. But this is:
- **Optional** (not automatic for all fields)
- **Only on `apply_configuration()` calls** (initial load and updates)
- **Not triggered by file extension**

---

## Safety Mechanisms and Constraints

### 1. Factory Reset on Version Mismatch
✓ **Explicit and immediate** - no silent data corruption
- Mismatch detected at startup before applying config
- Logged as VERBOSE: `"config version mismatch (%d vs %d), forcing reset."`
- Cannot be bypassed except with explicit `force=true` parameter

### 2. ACDI Data Preservation (Bytes 0-127)
✓ **Intentional** - preserves user identity (node name/description)
- Separate from application config
- Persists across version mismatches
- Lives at the standard ACDI user data offset

### 3. Event ID Regeneration on Reset
✓ **Automatic** - generates fresh event IDs
```cpp
void factory_reset_all_events(const InternalConfigData &cfg, 
                              uint64_t node_id, int fd)
{
    // Reads existing event counter, increments by # of producer/consumer entries
    uint16_t new_next_event = cfg.next_event().read(fd);
    // Generates new EventIDs: node_id << 16 | next_event_counter
    for (unsigned i = 0; cdi_event_offsets_ptr[i]; ++i)
    {
        EventId id = node_id;
        id <<= 16;
        id |= next_event++;
        EventConfigEntry(cdi_event_offsets_ptr[i]).write(fd, id);
    }
}
```
- Each reset gets fresh event numbers
- No collision risk with old event IDs

### 4. Callback on Factory Reset
✓ **Components notified** - can perform cleanup
```cpp
void configUpdateFlow_.factory_reset()
{
    // Iterates all registered ConfigUpdateListener instances
    // Calls listener->factory_reset(fd) for each
    // Each component writes its default values via CDI_FACTORY_RESET macro
}
```

Example from ServoConsumer:
```cpp
void factory_reset(int fd) OVERRIDE
{
    cfg_.description().write(fd, "");
    CDI_FACTORY_RESET(cfg_.servo_min_percent);  // Writes default to file
    CDI_FACTORY_RESET(cfg_.servo_max_percent);
}
```

---

## Data Preservation vs. Reset

| Item | Preserved | Reset | Details |
|------|-----------|-------|---------|
| **Node Name** | ✓ | - | ACDI bytes 0-127 preserved |
| **Node Description** | ✓ | - | ACDI bytes 0-127 preserved |
| **Config Version** | - | ✓ | Rewritten to expected_version |
| **Event IDs** | - | ✓ | New IDs generated |
| **Producer/Consumer Config** | - | ✓ | Erased (0xFF), then factory defaults written |
| **Custom App Config** | - | ✓ | Erased (0xFF), then listener's factory_reset() called |
| **Event Counter** | - | ✓ | Reset to 0 |
| **ACDI Version Byte** | - | ✓ | Rewritten to 2 |

---

## Limiting Factors and Gotchas

### 1. **No Partial Reset**
- Can't bump version for just one new field
- Any version mismatch = complete wipe
- Workaround: Use reserved padding space in segments (mentioned in LCC Technical Introduction)

### 2. **0xFF Extension Problem**
- If file is extended without version bump, new space is 0xFF
- Application config code must either:
  - Expect 0xFFFFFFFF as a sentinel value
  - Use `CDI_READ_TRIMMED()` to apply defaults
  - Check that `initial_load=true` and apply defaults in `apply_configuration()`
- **Not automatic** - requires defensive application code

### 3. **File Too Short Edge Case**
```cpp
if ((long)statbuf.st_size < (long)cfg.version().end_offset())
{
    LOG(VERBOSE, "%s is too short (%d vs %d), forcing reset.",
        CONFIG_FILENAME, (int)statbuf.st_size, cfg.version().end_offset());
    reset = true;
}
```
- If config file is corrupted and too short to hold version field
- Triggers reset even if version bytes are readable
- Prevents crashes from reading uninitialized memory

### 4. **SNIP_DYNAMIC_FILENAME Assumption**
```cpp
HASSERT(SNIP_DYNAMIC_FILENAME == CONFIG_FILENAME);
```
- Code assumes node name/description stored at start of CONFIG file
- If using separate SNIP file, this assertion fails
- Not true separation of concerns

### 5. **No Migration Window**
- When version mismatch detected, reset is **immediate**
- No opportunity to copy data manually
- Application can't rescue old config values
- Design assumes versioning is rare in production

---

## Configuration Update Flow on Version Mismatch

1. **Startup**: `create_config_file_if_needed(expected_version)`
2. **Check**: Read version from file
3. **Detect**: `if (read_version != expected_version) → reset = true`
4. **Preserve**: Save first 128 bytes (ACDI)
5. **Erase**: Fill rest with 0xFF
6. **Initialize**: 
   - Write version
   - Reset event counter
   - Generate new event IDs
7. **Notify**: `configUpdateFlow_.factory_reset()`
   - Calls each registered listener's `factory_reset(fd)`
   - Components write their own defaults
8. **Trigger**: `trigger_update()`
   - Calls all listeners' `apply_configuration(fd, initial_load=true, done)`
   - Components read config and apply

---

## Recommendations for T4.2 Documentation

When documenting config versioning for users:

### DO:
- ✓ Explain CANONICAL_VERSION is schema version, not firmware version
- ✓ Document that any version mismatch = factory reset
- ✓ Show how to use reserved padding to avoid version bumps
- ✓ Explain that node name survives version bumps
- ✓ Show listener's `apply_configuration()` is called with `initial_load=true`

### DON'T:
- ✗ Claim partial field migration is possible
- ✗ Suggest adding new required fields is backward compatible
- ✗ Expect uninitialized space (0xFF) to have safe default values
- ✗ Assume applying config happens automatically (listeners must implement it)

### Key Insight for Users:
> "CANONICAL_VERSION is all-or-nothing. Every node gets factory reset when it changes. Plan config layout carefully: add padding bytes if you expect future growth without losing user data."

---

## Source Files Consulted

- [OpenMRNLite/src/openlcb/SimpleStack.cpp](OpenMRNLite/src/openlcb/SimpleStack.cpp) - Version checking and factory reset
- [OpenMRNLite/src/openlcb/ConfigUpdateFlow.hxx](OpenMRNLite/src/openlcb/ConfigUpdateFlow.hxx) - Listener notification flow
- [OpenMRNLite/src/utils/ConfigUpdateListener.hxx](OpenMRNLite/src/utils/ConfigUpdateListener.hxx) - Listener interface
- [OpenMRNLite/src/openlcb/ConfigRepresentation.hxx](OpenMRNLite/src/openlcb/ConfigRepresentation.hxx) - CDI_FACTORY_RESET macro definition
- [OpenMRNLite/src/openlcb/ServoConsumer.hxx](OpenMRNLite/src/openlcb/ServoConsumer.hxx) - Example apply_configuration/factory_reset
