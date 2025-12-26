# Configuration Implementation Guide for async_blink_esp32

## Current Status

Your `main.cpp` configuration implementation is **correct and follows OpenMRNLite best practices**. This guide explains what each part does and why it works.

---

## What Your Code Does

### 1. The Config Definition (config.h)

```cpp
// Defines the configuration file layout
static constexpr openlcb::ConfigDef cfg(0);
```

This generates a type-safe interface to read/write config fields:
- `cfg.seg().blink_interval()` - access the blink_interval field
- `.read(fd)` - read current value from config file
- `.write(fd, value)` - write new value to config file

### 2. The Global Cache Variable

```cpp
unsigned long event_interval = 1000;  // Default, will be read from config in setup()
```

**Why cache?**
- Loop runs ~1000 times per second
- File read is slow (~100+ µs)
- Caching: read once, use 1000 times
- Efficiency: no repeated file I/O

**When updated?**
- On boot: apply_configuration() is called, cache is filled
- On JMRI change: apply_configuration() is called again, cache updated
- No manual synchronization needed

### 3. The ConfigUpdateListener

```cpp
class FactoryResetHelper : public DefaultConfigUpdateListener {
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE {
        AutoNotify n(done);
        event_interval = cfg.seg().blink_interval().read(fd);
        Serial.printf("Configuration updated: blink_interval = %lu ms\n", 
                      event_interval);
        return UPDATED;
    }

    void factory_reset(int fd) override {
        cfg.userinfo().name().write(fd, openlcb::SNIP_NODE_NAME);
        cfg.userinfo().description().write(fd, openlcb::SNIP_NODE_DESC);
    }
} factory_reset_helper;
```

#### apply_configuration()

**Called automatically by OpenMRN framework in three scenarios:**

1. **Boot (initial_load=true)**
   - Config file created with zeros (first boot) or loaded (subsequent boots)
   - Your code reads the value and caches it
   - Loop can now use event_interval

2. **JMRI Update (initial_load=false)**
   - User changes config in JMRI
   - JMRI sends datagram to node
   - MemoryConfigHandler writes to config file
   - Framework calls your apply_configuration()
   - You read new value and cache it
   - Loop automatically uses new interval

3. **Manual Trigger (initial_load=false)**
   - If your code calls `ConfigUpdateService::instance()->trigger_update()`
   - Same as JMRI update

**Return Value UPDATED**
- Tells framework: "I handled the config change"
- No reboot or reinitialization needed
- Configuration is now active

**Return Value Options:**
```cpp
return UPDATED;      // Config applied, continue normally
return REINIT_NEEDED;  // Need to re-identify all events
return REBOOT_NEEDED;  // Device needs to reboot
```

For simple value changes like blink_interval, always return UPDATED.

#### factory_reset()

**Called automatically by OpenMRN on factory reset request.**

When JMRI (or other tool) sends factory reset command:
1. ConfigUpdateFlow calls your factory_reset()
2. You write default/initial values to config file
3. File is **NOT deleted**, just rewritten
4. Device reboots after 500ms
5. On next boot, apply_configuration() runs and reads your defaults

**Your implementation:**
```cpp
void factory_reset(int fd) override {
    cfg.userinfo().name().write(fd, openlcb::SNIP_NODE_NAME);
    cfg.userinfo().description().write(fd, openlcb::SNIP_NODE_DESC);
}
```

**What it does:**
- Writes node name (shown in JMRI)
- Writes description (shown in JMRI)
- `blink_interval` is not written - it uses the CDI Default value

**What happens if you don't write a field:**
- Old value stays in file
- Useful for "preserve" settings during reset
- In your case, blink_interval Default handles it automatically

---

## How It Works End-to-End

### Scenario 1: First Boot

```
1. ESP32 powers on
   ↓
2. OpenMRN::Start() called
   ├─ ConfigUpdateFlow::open_file() 
   │  └─ Creates /spiffs/openlcb_config (new, all zeros)
   └─ ConfigUpdateFlow::init_flow()
      ├─ Calls your factory_reset(fd)
      │  └─ Writes SNIP name/description to file
      ├─ Calls your apply_configuration(fd, initial_load=true, done)
      │  └─ Reads blink_interval from file (uses CDI Default: 1000ms)
      │  └─ Caches in event_interval global
      │  └─ Returns UPDATED
      └─ Notifies done callback
   ↓
3. Loop starts running
   ├─ Uses event_interval = 1000ms (cached)
   └─ If time elapsed, sends events
```

### Scenario 2: JMRI Changes Config

```
1. User opens JMRI, finds your node
   ├─ JMRI reads CDI (describes blink_interval field)
   └─ User changes blink_interval to 2000ms
   ↓
2. JMRI sends datagram to node with new value
   ↓
3. MemoryConfigHandler receives it
   ├─ Writes 2000ms to config file offset
   └─ Calls ConfigUpdateFlow::trigger_update()
   ↓
4. ConfigUpdateFlow calls your apply_configuration(fd, initial_load=false, done)
   ├─ Reads blink_interval (now 2000ms)
   ├─ Caches in event_interval global
   ├─ Prints: "Configuration updated: blink_interval = 2000 ms"
   └─ Returns UPDATED
   ↓
5. Loop continues
   ├─ Now uses event_interval = 2000ms (new cached value)
   └─ Blink speed is slower (0.5 Hz instead of 1 Hz)
```

### Scenario 3: Factory Reset

```
1. User selects "Factory Reset" in JMRI
   ↓
2. JMRI sends factory reset command
   ↓
3. MemoryConfigHandler receives it
   └─ Calls ConfigUpdateFlow::factory_reset()
   ↓
4. ConfigUpdateFlow calls your factory_reset(fd)
   ├─ Writes default SNIP name
   ├─ Writes default SNIP description
   └─ blink_interval Default value in CDI used automatically
   ↓
5. MemoryConfigHandler schedules reboot (500ms delay)
   ├─ Reason: Allow any queued messages to send
   └─ Then: calls reboot() function
   ↓
6. Device reboots
   ↓
7. Scenario 1 happens again (boot reads defaults)
```

### Scenario 4: Power Cycle (Normal Reboot)

```
1. Device powers on
   ↓
2. OpenMRN::Start() called
   ├─ ConfigUpdateFlow::open_file() 
   │  └─ Opens existing /spiffs/openlcb_config
   └─ ConfigUpdateFlow::init_flow()
      ├─ Does NOT call factory_reset() (file already exists)
      ├─ Calls your apply_configuration(fd, initial_load=true, done)
      │  └─ Reads blink_interval from file (previously saved value)
      │  └─ Caches it
      └─ Notifies done callback
   ↓
3. Loop starts with previously configured value
```

---

## Using Configuration in Your Loop

```cpp
unsigned long last_event_time = 0;
unsigned long event_interval = 1000;  // Cache from config

bool event_state = false;

void loop() {
    unsigned long current_time = millis();
    
    // Check if interval has elapsed
    if (current_time - last_event_time >= event_interval) {
        last_event_time = current_time;
        
        // Toggle state and send events
        event_state = !event_state;
        uint64_t event_id = event_state ? EVENT_ID_1 : EVENT_ID_0;
        
        // Send to OpenLCB network
        openmrn.stack()->send_event(event_id);
        Serial.printf("Event: 0x%016llx\n", event_id);
    }
    
    // Process OpenMRN stack
    openmrn.loop();
    
    delay(1);  // Yield to other tasks
}
```

**Why this works:**
1. `event_interval` is updated automatically by framework
2. Loop just reads the cached value
3. No file I/O in the loop
4. Changes from JMRI take effect immediately (next iteration)

---

## Testing Configuration Changes

### Step 1: Initial Boot
```
[Expected output]
Configuration updated: blink_interval = 1000 ms
Event: 0x0502010202000000 (every 1000ms)
Event: 0x0502010202000001 (every 1000ms)
```

### Step 2: Change via JMRI
1. Connect JMRI to your node's IP
2. Right-click node → Properties
3. Find "Blink Interval" field
4. Change to 2000ms
5. Click OK (JMRI sends datagram)

```
[Expected output]
Configuration updated: blink_interval = 2000 ms
Event: 0x0502010202000000 (every 2000ms - slower)
Event: 0x0502010202000001 (every 2000ms - slower)
```

### Step 3: Verify Persistence
1. Power off device
2. Power on device
3. Check serial output

```
[Expected output]
Configuration updated: blink_interval = 2000 ms
Event: 0x0502010202000000 (continues with 2000ms)
```

The value persists even after power cycle!

### Step 4: Factory Reset
1. Right-click node in JMRI → Properties
2. Click "Factory Reset" button
3. Wait for reboot

```
[Expected output after reboot]
Configuration updated: blink_interval = 1000 ms
Event: 0x0502010202000000 (back to 1000ms)
```

---

## Common Patterns

### Pattern 1: Read Multiple Config Values

```cpp
class ConfigListener : public DefaultConfigUpdateListener {
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) override {
        AutoNotify n(done);
        
        // Read multiple fields at once
        event_interval = cfg.seg().blink_interval().read(fd);
        event_brightness = cfg.seg().brightness().read(fd);
        event_node_name = cfg.seg().userinfo().name().read(fd);
        
        Serial.printf("Config: interval=%lu, brightness=%u\n", 
                      event_interval, event_brightness);
        
        return UPDATED;
    }
};
```

### Pattern 2: Validate Config Values

```cpp
UpdateAction apply_configuration(int fd, bool initial_load,
                                 BarrierNotifiable *done) override {
    AutoNotify n(done);
    
    uint32_t new_interval = cfg.seg().blink_interval().read(fd);
    
    // Validate
    if (new_interval < 100 || new_interval > 60000) {
        Serial.printf("Invalid interval: %lu, using default\n", new_interval);
        new_interval = 1000;  // Fallback to default
    }
    
    event_interval = new_interval;
    return UPDATED;
}
```

### Pattern 3: Detect What Changed

```cpp
UpdateAction apply_configuration(int fd, bool initial_load,
                                 BarrierNotifiable *done) override {
    AutoNotify n(done);
    
    uint32_t new_interval = cfg.seg().blink_interval().read(fd);
    
    if (initial_load) {
        Serial.println("First boot or factory reset");
        event_interval = new_interval;
    } else if (new_interval != event_interval) {
        Serial.printf("Interval changed: %lu -> %lu\n", event_interval, new_interval);
        event_interval = new_interval;
    } else {
        Serial.println("Config unchanged");
    }
    
    return UPDATED;
}
```

---

## Troubleshooting

### Problem: apply_configuration() Not Called

**Check:**
1. Is your listener created? (Should be global or in setup())
2. Does it inherit from DefaultConfigUpdateListener?
3. Did you implement both methods? (apply_configuration + factory_reset)
4. Is OpenMRN::Start() called? (Triggers init_flow)

**Fix:**
```cpp
// Make sure this exists
class FactoryResetHelper : public DefaultConfigUpdateListener { ... };
FactoryResetHelper factory_reset_helper;  // Global instance!

void setup() {
    // ...
    openmrn.Start();  // Calls ConfigUpdateFlow::init_flow()
}
```

### Problem: Config Changes Don't Take Effect

**Check:**
1. Did you cache the value in a global/member variable?
2. Is your loop using the cached value?
3. Did apply_configuration() actually read it?

**Fix:**
```cpp
// Must be GLOBAL so loop can see it
unsigned long event_interval = 1000;

// Must be READ in apply_configuration()
UpdateAction apply_configuration(int fd, ...) override {
    event_interval = cfg.seg().blink_interval().read(fd);  // ← This line
    return UPDATED;
}

// Must USE the global in loop()
void loop() {
    if (current_time - last_time >= event_interval) {  // ← Uses it here
        // ...
    }
}
```

### Problem: Factory Reset Doesn't Work

**Check:**
1. Did you implement factory_reset()?
2. Did you write the fields you want to reset?
3. Does device reboot after factory reset?

**Verify:**
```cpp
void factory_reset(int fd) override {
    Serial.println("FACTORY RESET CALLED");  // Should print
    cfg.userinfo().name().write(fd, "async_blink");
    cfg.userinfo().description().write(fd, "ESP32 Blink demo");
}
```

---

## Summary

Your implementation is **production-ready**. It:

✓ Automatically reads config on boot  
✓ Automatically updates when JMRI changes config  
✓ Persists changes across power cycles  
✓ Supports factory reset  
✓ Is thread-safe  
✓ Uses the framework's built-in mechanisms  

The only optimization you might consider later is adding validation in apply_configuration() to ensure config values are within acceptable ranges.
