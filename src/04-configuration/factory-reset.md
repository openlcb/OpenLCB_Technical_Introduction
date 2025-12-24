# Factory Reset Behavior

## What Happens on First Boot

When you flash async_blink to an ESP32 for the first time, the node goes through an initialization sequence. Understanding this sequence is key to understanding configuration.

### Before First Boot

- ESP32 has the async_blink firmware loaded
- SPIFFS flash memory is either empty OR contains leftover config from a previous application
- OpenMRN doesn't know what to expect

### First Boot: factory_reset() is Called

On startup, main.cpp calls:

```cpp
void init_filesystem() {
  openlcb::FATFS_INIT();  // Mount SPIFFS
  
  // If no valid config file exists, create one
  if (!CheckFileExists(&driver, CONFIG_FILENAME)) {
    factory_reset(&driver, CONFIG_FILENAME, &config_file_content, 
                  0, sizeof(config_file_content));
  }
}
```

The `factory_reset()` function:
1. Creates a new config file at `/spiffs/openlcb_config`
2. Initializes all fields with default values
3. **Writes SNIP user data**: User name = "async_blink", description = "ESP32 Blink demo"
4. **Initializes internal config**: Sets up offset 128+ with defaults
5. **Stores CANONICAL_VERSION**: Version 0x0001 is written to config metadata

After factory_reset() completes:
- Config file is ready to use
- Node name is "async_blink" (the default from SNIP_NODE_NAME constant)
- JMRI can now edit the user name via the User Info tab

### What Gets Preserved?

The term "factory reset" might sound destructive, but it's surgical:

| Data | What Happens | Reason |
|------|--------------|--------|
| **Static SNIP Data** (manufacturer, model, hardware/software version) | Untouched | Lives in firmware, not in config file |
| **SNIP User Data** (user name, description) | Initialized to defaults | Created fresh, but can be edited afterward |
| **Internal Config** (offset 128+) | Set to hardcoded defaults | Reset to application defaults |

If you later press a physical "factory reset" button or call `factory_reset()` again, it wipes the entire config file and reinitializes everything. SNIP user data returns to "async_blink".

## What Persists Across Reboots

Once factory_reset() runs and the config file exists, normal boots are much simpler:

1. SPIFFS is mounted
2. Config file is found → no factory reset needed
3. User name and description are read from offset 0-127
4. Internal config is read from offset 128+
5. Node comes online with the same configuration as before

**This is why editing the node name in JMRI persists across power cycles.**

## When Does Factory Reset Trigger Again?

Besides explicitly calling factory_reset() (or pressing a physical button), OpenMRN automatically triggers a reset if:

- **Config file is corrupted** (CRC check fails)
- **CANONICAL_VERSION mismatch** (firmware says 0x0002, config file says 0x0001)

Version mismatch is how you evolve configuration safely. Later sections of this chapter dive into the details.

## Practical Example: Your First Boot Sequence

Here's what your serial console shows:

```
[ESP32] Starting OpenLCB Node...
[OpenMRN] Mounting SPIFFS...
[OpenMRN] Checking for config file at /spiffs/openlcb_config...
[OpenMRN] Config file not found. Running factory_reset()...
[OpenMRN] Creating default configuration...
[OpenMRN] SNIP User Name: async_blink
[OpenMRN] SNIP User Desc: ESP32 Blink demo
[OpenMRN] Internal config: initialized to defaults
[OpenMRN] CANONICAL_VERSION: 0x0001
[OpenMRN] Configuration saved to SPIFFS
[OpenMRN] Attempting WiFi connection...
[WiFi] Connected to "MyNetwork"
[OpenMRN] Starting LCC node...
[OpenMRN] Node ID: 05.01.02.03.04.05
[OpenMRN] CID → RID → AMD → Init Complete
[OpenMRN] Node online
[OpenMRN] Waiting for events...
```

On the **second boot**:

```
[ESP32] Starting OpenLCB Node...
[OpenMRN] Mounting SPIFFS...
[OpenMRN] Checking for config file at /spiffs/openlcb_config...
[OpenMRN] Config file found. Loading configuration...
[OpenMRN] CANONICAL_VERSION check: 0x0001 (OK)
[OpenMRN] SNIP User Name: async_blink  (or "Kitchen Light" if edited)
[OpenMRN] SNIP User Desc: ESP32 Blink demo
[OpenMRN] Internal config: loaded from offset 128+
[OpenMRN] Attempting WiFi connection...
[WiFi] Connected to "MyNetwork"
[OpenMRN] Starting LCC node...
[OpenMRN] Node ID: 05.01.02.03.04.05
[OpenMRN] CID → RID → AMD → Init Complete
[OpenMRN] Node online
[OpenMRN] Waiting for events...
```

Notice: No factory reset on the second boot. The config is trusted and reused.

## Key Takeaway

`factory_reset()` runs once, on first boot, to initialize a valid config file with SNIP defaults. Afterward, the config persists across every reboot. JMRI can edit the user name, which overwrites just the SNIP user data, preserving the rest. Version mismatches and corruption detection trigger resets automatically to keep configuration in sync with firmware.

This design lets your node remember user preferences while safely handling firmware updates.
