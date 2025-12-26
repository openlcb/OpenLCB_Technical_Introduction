# Reading Configuration at Runtime in OpenMRNLite

## Overview

This document explains how to read configuration values at runtime in OpenMRNLite, specifically for non-GPIO-bound values that don't require OpenLCB event producers/consumers. It covers the configuration lifecycle, file descriptor system, and persistence mechanisms.

**Key Finding**: The user's approach is **correct** and represents the standard pattern used throughout OpenMRNLite for runtime configuration access.

---

## 1. Configuration File Descriptor System

### Getting the Configuration FD

The configuration file is managed by `ConfigUpdateFlow`, which is a state machine that handles the entire configuration lifecycle. The file descriptor can be accessed in two ways:

#### **Method 1: Via apply_configuration() callback (Recommended)**

```cpp
class MyConfigListener : public DefaultConfigUpdateListener
{
public:
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        
        // fd is provided as a parameter
        my_config_value = cfg.seg().my_field().read(fd);
        
        return UPDATED;
    }

    void factory_reset(int fd) OVERRIDE
    {
        // ... reset config to defaults
    }
} my_listener;
```

**Advantages**:
- Called automatically whenever configuration is loaded or updated
- Thread-safe (called on the main executor)
- Framework handles file lifecycle
- Works for both initial boot and JMRI-driven config changes

#### **Method 2: Directly via memory_config_handler (Runtime Access)**

```cpp
int config_fd = openmrn.stack()->memory_config_handler()->get_storage()->get_fd();
if (config_fd >= 0) {
    my_value = cfg.seg().my_field().read(config_fd);
}
```

**Advantages**:
- Can read config at any time during runtime
- Does not require waiting for config update cycle

**Important Limitations**:
- The fd may be -1 if config file hasn't been opened yet
- Race conditions possible if config is being updated simultaneously
- Should only be used after initial configuration load completes

### The `get_storage()` Interface

The `memory_config_handler()->get_storage()` returns a `MemorySpace*` object (typically `FileMemorySpace`). This interface provides:

```cpp
// From MemoryConfig.hxx
class FileMemorySpace : public MemorySpace
{
public:
    static const address_t AUTO_LEN = (address_t) - 1;
    static const address_t UNLIMITED_LEN = (address_t) - 2;

    // Creates a memory space from an open file descriptor
    FileMemorySpace(int fd, address_t len = AUTO_LEN);
    
    // Creates a memory space from a file path
    FileMemorySpace(const char *name, address_t len = AUTO_LEN);

    // Read/write operations
    size_t read(address_t source, uint8_t *dst, size_t len, 
                errorcode_t *error, Notifiable *again) override;
    size_t write(address_t destination, const uint8_t *data, size_t len,
                 errorcode_t *error, Notifiable *again) override;
};
```

**Available Methods**:
- `get_fd()`: Returns the file descriptor (-1 if not open)
- `read()`, `write()`: Low-level memory access
- CDI accessors like `cfg.seg().field().read(fd)`: High-level typed access

---

## 2. Configuration Lifecycle and File Persistence

### Complete Boot Sequence

```
Boot
  ↓
ConfigUpdateFlow opens config file (or creates if missing)
  ↓
For each registered ConfigUpdateListener:
  ├─ Call factory_reset(fd) if config file is NEW (first boot)
  └─ Call apply_configuration(fd, initial_load=true, done)
  ↓
Loop: Listen for config changes via MemoryConfigHandler
  ├─ JMRI/external tool modifies config
  ├─ ConfigUpdateFlow gets trigger_update() call
  ├─ For each listener: apply_configuration(fd, initial_load=false, done)
  └─ If REINIT_NEEDED: perform node reinitialization
  ↓
Loop continues until factory reset requested
```

### Automatic vs Manual Saving

**Configuration changes are automatically saved** through the OpenLCB MemoryConfig protocol:

1. **JMRI sends a datagram** with new configuration values
2. **MemoryConfigHandler receives it** and writes to the config file
3. **MemoryConfigHandler then calls** `ConfigUpdateFlow::trigger_update()`
4. **trigger_update()** notifies all registered listeners via `apply_configuration()`

**Your application code does NOT need to manually save**. The framework handles all file I/O.

**Proof from source**:
```cpp
// From MemoryConfig.cxx
// When a write command arrives from the bus:
size_t FileMemorySpace::write(address_t destination, const uint8_t *data,
                              size_t len, errorcode_t *error, Notifiable *again)
{
    // File is written immediately
    lseek(fd_, destination, SEEK_SET);
    ::write(fd_, data, len);  // Direct file write
    // No explicit fsync() - relies on OS buffering
    return len;
}
```

### Reading Configuration Values

The CDI framework provides type-safe accessors for all config fields:

```cpp
// Reading different types
uint8_t delay = cfg.seg().my_delay().read(fd);
uint16_t interval = cfg.seg().my_interval().read(fd);
uint64_t event_id = cfg.seg().my_event().read(fd);
std::string name = cfg.seg().my_name().read(fd);

// Reading from repeated groups
for (unsigned i = 0; i < cfg.seg().consumers().num_repeats(); ++i) {
    EventId event = cfg.seg().consumers().entry(i).event().read(fd);
}
```

**How it works internally**:
1. Accessor object knows the offset in the config file
2. `read()` performs `lseek(fd, offset, SEEK_SET)`
3. Reads bytes from file and deserializes to typed value
4. Type safety enforced at compile time via templates

---

## 3. Factory Reset Behavior and File Recreation

### What factory_reset() Does

When factory reset is triggered (externally via MemoryConfig protocol):

```cpp
// From MemoryConfig.cpp - handle_factory_reset()
uint16_t MemoryConfigHandler::handle_factory_reset(NodeID target)
{
    // Call ConfigUpdateFlow to trigger factory reset
    static_cast<ConfigUpdateFlow *>(ConfigUpdateFlow::instance())
        ->factory_reset();
    
    // Schedule reboot after delay
    (new RebootTimer(service()))
        ->start(MSEC_TO_NSEC(FACTORY_RESET_REBOOT_DELAY_MSEC));
    
    return 0;
}

// From ConfigUpdateFlow.cpp
void ConfigUpdateFlow::factory_reset()
{
    // Call every listener's factory_reset() method
    for (auto it = listeners_.begin(); it != listeners_.end(); ++it) {
        it->factory_reset(fd_);  // Same config file fd
    }
}
```

### Configuration File Handling

**Key points**:
1. Config file is **NOT deleted** by factory reset
2. File is **reused** on next boot
3. **factory_reset()** should write default values to the existing file
4. If a field is not explicitly reset, its old value persists

**Example**:
```cpp
void factory_reset(int fd) override
{
    // These writes clear user-modifiable fields to defaults
    cfg.userinfo().name().write(fd, "Default Name");
    cfg.userinfo().description().write(fd, "Default Description");
    
    // Important: If you don't write a field, it keeps its old value!
    // This can be used intentionally to preserve some settings
}
```

### First Boot vs. Subsequent Boots

**First Boot** (`initial_load=true`):
- Config file doesn't exist
- Framework creates it with zeros
- `factory_reset()` is called first
- Then `apply_configuration(..., initial_load=true, ...)`
- Application should initialize from defaults

**Subsequent Boots** (`initial_load=false` for updates):
- Config file exists with saved values
- `apply_configuration(..., initial_load=false, ...)` called
- Application reads saved values
- `factory_reset()` is NOT called (unless user requests it)

---

## 4. Real-World Examples from OpenMRNLite

### Example 1: ConfiguredConsumer Pattern (GPIO Consumer)

```cpp
// From OpenMRNLite/src/openlcb/ConfiguredConsumer.hxx

class ConfiguredConsumer : public ConfigUpdateListener
{
public:
    ConfiguredConsumer(Node *node, const ConsumerConfig &cfg, const Gpio *gpio)
        : impl_(node, 0, 0, gpio), consumer_(&impl_), cfg_(cfg)
    {
        // Auto-register with ConfigUpdateService
        ConfigUpdateService::instance()->register_update_listener(this);
    }

    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        
        // Read event IDs from config file
        EventId cfg_event_on = cfg_.event_on().read(fd);
        EventId cfg_event_off = cfg_.event_off().read(fd);
        
        if (cfg_event_off != impl_.event_off() ||
            cfg_event_on != impl_.event_on())
        {
            // Reinitialize with new event IDs
            auto saved_gpio = impl_.gpio_;
            auto saved_node = impl_.node();
            consumer_.~BitEventConsumer();
            impl_.Impl::~Impl();
            new (&impl_) Impl(saved_node, cfg_event_on, cfg_event_off, saved_gpio);
            new (&consumer_) BitEventConsumer(&impl_);
            return REINIT_NEEDED;  // Triggers event identify
        }
        return UPDATED;
    }

    void factory_reset(int fd) OVERRIDE
    {
        // Initialize description field to empty string
        cfg_.description().write(fd, "");
    }
};
```

**Key Pattern**:
- Register in constructor
- Read config in `apply_configuration()`
- Compare old vs new to detect changes
- Return `REINIT_NEEDED` if reinitialization required
- Write defaults in `factory_reset()`

### Example 2: Simple Non-GPIO Configuration (Your Pattern)

```cpp
// From OpenLCB_Technical_Introduction/async_blink_esp32/main.cpp

// Global state variable
unsigned long event_interval = 1000;  // Default

class FactoryResetHelper : public DefaultConfigUpdateListener
{
public:
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        
        // Read from config file into global variable
        event_interval = cfg.seg().blink_interval().read(fd);
        Serial.printf("Configuration updated: blink_interval = %lu ms\n", 
                      event_interval);
        
        return UPDATED;
    }

    void factory_reset(int fd) override
    {
        // Initialize SNIP dynamic data (shown in JMRI)
        cfg.userinfo().name().write(fd, openlcb::SNIP_NODE_NAME);
        cfg.userinfo().description().write(fd, openlcb::SNIP_NODE_DESC);
        // Note: blink_interval has a Default() in CDI, so no need to reset
    }
} factory_reset_helper;

// In loop() or similar:
unsigned long current_time = millis();
if (current_time - last_event_time >= event_interval) {
    // Use the cached event_interval value
    last_event_time = current_time;
    // ... send events ...
}
```

**Why this pattern works**:
1. `apply_configuration()` is called on every boot and on JMRI changes
2. Global `event_interval` is updated
3. Loop code uses cached value (no repeated file reads)
4. On next JMRI change, framework calls `apply_configuration()` again

---

## 5. Alternative Access Methods

### Method A: ConfigUpdateListener (Recommended)
✓ Automatic callbacks  
✓ Guaranteed consistency  
✓ Thread-safe  
✓ Works for both initial load and updates  

```cpp
class MyListener : public DefaultConfigUpdateListener {
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) {
        // Read config here
        my_value = cfg.seg().field().read(fd);
        return UPDATED;
    }
};
```

### Method B: Direct get_storage() Access
✓ Can read at any time  
✓ No registration needed  
✗ Requires null checking  
✗ Potential race conditions  

```cpp
int fd = openmrn.stack()->memory_config_handler()->get_storage()->get_fd();
if (fd >= 0) {
    my_value = cfg.seg().field().read(fd);
}
```

### Method C: Store Value in Listener
✓ Clean separation of concerns  
✓ Automatic updates  
✓ Type-safe  
✓ No global state  

```cpp
class ConfigHolder : public DefaultConfigUpdateListener {
private:
    uint32_t interval_;
public:
    uint32_t get_interval() const { return interval_; }
    
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) {
        AutoNotify n(done);
        interval_ = cfg.seg().interval().read(fd);
        return UPDATED;
    }
};

ConfigHolder config_holder;
// Later:
use_interval(config_holder.get_interval());
```

---

## 6. ConfigUpdateService Architecture

### The Update Flow

```
┌─────────────────────────────────────┐
│   ConfigUpdateFlow (State Machine)  │
│  - Manages config file descriptor   │
│  - Dispatches config updates        │
│  - Triggers reboots/reinits         │
└─────────────────┬───────────────────┘
                  │
        ┌─────────┴─────────┬─────────┬───────────┐
        │                   │         │           │
        ▼                   ▼         ▼           ▼
    Listener1           Listener2  Listener3  Listener4
 (ConfiguredProducer) (Consumer) (Custom)   (SNIP Helper)
    
    Each Listener:
    ├─ register_update_listener() in constructor
    ├─ receive apply_configuration(fd, ...) callback
    ├─ read/write config via fd
    └─ return UPDATED/REINIT_NEEDED/REBOOT_NEEDED
```

### Available Methods on ConfigUpdateFlow

```cpp
class ConfigUpdateFlow : public ConfigUpdateService {
public:
    // Called during stack initialization
    int open_file(const char *path);
    void init_flow();
    
    // Called on factory reset request
    void factory_reset();
    
    // Get current FD at any time
    int get_fd() { return fd_; }
    
    // Trigger config check (called by MemoryConfigHandler)
    void trigger_update() override;
    
    // Register/unregister listeners
    void register_update_listener(ConfigUpdateListener *listener) override;
    void unregister_update_listener(ConfigUpdateListener *listener) override;
};
```

---

## 7. Thread Safety and Concurrency

### ConfigUpdateFlow is Atomic

The ConfigUpdateFlow uses `AtomicHolder` to protect:
- The listeners queue
- The refresh iterator
- The config file descriptor
- The "needs reboot/reinit" flags

### Safe Pattern

```cpp
class SafeConfigListener : public DefaultConfigUpdateListener {
public:
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) override {
        AutoNotify n(done);
        
        // This code runs on the main executor
        // Safe to read/write shared state that's only modified here
        int new_value = cfg.seg().field().read(fd);
        
        // Safe to update a member variable
        my_value_ = new_value;
        
        return UPDATED;
    }
    
private:
    int my_value_;
};
```

### Rules

1. **`apply_configuration()` runs on the main executor** - no concurrent access
2. **Don't use mutexes in apply_configuration()** - deadlock risk
3. **Read config values immediately** - don't cache the fd for later
4. **Store results in class members** - accessed from other threads only after apply returns

---

## 8. Configuration Update Triggers

### Initial Load on Boot
```
Stack::create()
  └─ ConfigUpdateFlow::init_flow()
     ├─ do_initial_load()  [load pending listeners]
     └─ For each: apply_configuration(..., initial_load=true, ...)
```

### Update from JMRI
```
MemoryConfigHandler receives write datagram
  └─ FileMemorySpace::write(fd, ...)  [write to file]
  └─ ConfigUpdateFlow::trigger_update()
     └─ For each listener: apply_configuration(..., initial_load=false, ...)
```

### Factory Reset
```
MemoryConfigHandler receives factory reset datagram
  └─ ConfigUpdateFlow::factory_reset()
     └─ For each listener: factory_reset(fd)
     └─ MemoryConfigHandler schedules 500ms reboot
     └─ After reboot: init_flow() called again
```

### Manual Trigger
```cpp
// If your application needs to re-read config:
openmrn.stack()->config_update_flow()->trigger_update();
```

---

## 9. CDI Configuration Entry Types

All these can be read/written via config file:

```cpp
// Numeric types
Uint8ConfigEntry      // uint8_t
Uint16ConfigEntry     // uint16_t  
Uint32ConfigEntry     // uint32_t
Uint64ConfigEntry     // uint64_t
Int8ConfigEntry       // int8_t
Int16ConfigEntry      // int16_t
Int32ConfigEntry      // int32_t
Int64ConfigEntry      // int64_t

// OpenLCB types
EventConfigEntry      // uint64_t (event ID)
NodeConfigEntry       // uint64_t (node ID)

// Strings
StringConfigEntry<N>  // Fixed-length string (N bytes max)

// Examples
CDI_GROUP(MyConfig);
CDI_GROUP_ENTRY(interval, Uint32ConfigEntry, Default(1000));
CDI_GROUP_ENTRY(event, EventConfigEntry);
CDI_GROUP_ENTRY(name, StringConfigEntry<64>, Default(""));
CDI_GROUP_END();
```

Reading from file:
```cpp
uint32_t interval = cfg.seg().interval().read(fd);
uint64_t event_id = cfg.seg().event().read(fd);
std::string name = cfg.seg().name().read(fd);
```

---

## 10. Complete Minimal Example

```cpp
#include <OpenMRNLite.h>

// OpenMRN stack
static constexpr uint64_t NODE_ID = 0x050201020200ULL;
OpenMRN openmrn(NODE_ID);

// Configuration definition (in config.h)
static constexpr openlcb::ConfigDef cfg(0);

// State
uint32_t blink_interval = 1000;  // Cached from config

// Listen for config updates
class ConfigListener : public DefaultConfigUpdateListener {
public:
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) override {
        AutoNotify n(done);
        blink_interval = cfg.seg().blink_interval().read(fd);
        return UPDATED;
    }

    void factory_reset(int fd) override {
        cfg.userinfo().name().write(fd, "MyNode");
        cfg.userinfo().description().write(fd, "Test Node");
        // blink_interval has Default() so no need to reset
    }
} config_listener;

// In setup():
void setup() {
    openmrn.StartWifi("SSID", "password");
    openmrn.Start();  // Calls ConfigUpdateFlow::init_flow()
                      // Which calls apply_configuration()
}

// In loop():
void loop() {
    // Use cached value (no file access needed)
    if (/* time to blink */) {
        // Send OpenLCB events
        openmrn.SendEventReport(event_id);
    }
}
```

---

## Summary

### Is the User's Approach Correct?

**Yes.** The code:
```cpp
int config_fd = openmrn.stack()->memory_config_handler()->get_storage()->get_fd();
if (config_fd >= 0) {
    event_interval = cfg.seg().blink_interval().read(config_fd);
}
```

is valid but **not the recommended primary approach**. It works for:
- Reading configuration at arbitrary times
- Checking if config has been loaded yet
- Debugging configuration values

### Better Approach

Use `ConfigUpdateListener::apply_configuration()` as the primary mechanism:

```cpp
class MyConfigListener : public DefaultConfigUpdateListener {
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) override {
        AutoNotify n(done);
        event_interval = cfg.seg().blink_interval().read(fd);
        return UPDATED;
    }
    void factory_reset(int fd) override {
        cfg.userinfo().name().write(fd, "Default Name");
    }
} my_listener;
```

This ensures:
- ✓ Config is read automatically on boot
- ✓ Config is updated when JMRI changes it
- ✓ Thread-safe and synchronized with framework
- ✓ Handles initial_load flag properly
- ✓ No race conditions or fd availability issues

### Key Takeaways

1. **Files are managed automatically** - no manual open/close/save needed
2. **Factory reset rewrites defaults** but doesn't delete the file
3. **Updates are synchronous** - when apply_configuration() returns, changes are applied
4. **Use ConfigUpdateListener** for reliable, automatic config updates
5. **Use get_storage()->get_fd()** for ad-hoc runtime config reads (with null checking)
6. **Caching values** in apply_configuration() is the standard pattern
7. **REINIT_NEEDED** tells the framework when events need to be re-announced
