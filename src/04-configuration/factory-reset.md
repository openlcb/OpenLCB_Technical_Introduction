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

### What Gets Wiped and What Doesn't

When `factory_reset()` is triggered:

| Data | What Happens | Reason |
|------|--------------|--------|
| **Static SNIP Data** (manufacturer, model, hardware/software version) | Untouched | Lives in firmware, not in config file |
| **SNIP User Data** (user name, description) | **Completely wiped** | Reset to defaults from config.h (e.g., "async_blink") |
| **Internal Config** (offset 128+) | Completely wiped | Reset to application defaults |
| **User Customizations** (node name set via JMRI) | **Lost** | Factory reset erases all custom edits |

This is important to understand: factory reset is **all-or-nothing**. Once triggered, users lose any custom node name or description they may have set through JMRI. There is no selective preservation of custom fields.

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
- **CANONICAL_VERSION mismatch** (firmware version differs from saved config version)

**Important Warning**: If you change `CANONICAL_VERSION` in your firmware (e.g., to add new configuration fields), the next boot will trigger a factory reset. Users will lose any custom node name or configuration they set through JMRI. This is a breaking change for deployed nodes.

## Triggering a Factory Reset from JMRI

You can manually trigger a factory reset from the JMRI LccPro configure dialog without recompiling firmware or pressing buttons on the ESP32:

1. Open **LccPro** and select your node
2. Click **Configure** (or double-click the node)
3. In the configure dialog, click the **More...** button
4. Select **Factory Reset**

JMRI will send a factory reset command to the node via OpenLCB Configuration protocol. The node will immediately reinitialize its config file with factory defaults. This is useful for:

- **Testing**: Quickly reset configuration during development
- **Troubleshooting**: Recover from corrupted settings without physical access to the board
- **User Support**: Remotely help users restore a node to factory defaults without recompiling

## Practical Example: Factory Reset Reboot

When you trigger a factory reset (via JMRI or version mismatch), here's what the serial console shows on the reboot:

```
rst:0xc (SW_CPU_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:1184
load:0x40078000,len:13232
load:0x40080400,len:3028
entry 0x400805e4

=== OpenLCB async_blink ESP32 Example ===
Node ID: 0x050201020200
Event 0: 0x0502010202000000
Event 1: 0x0502010202000001

Initializing SPIFFS...
SPIFFS initialized successfully

Connecting to WiFi SSID: YourNetwork
.
WiFi connected!
IP Address: 192.168.1.100

Creating CDI configuration descriptor...
[CDI] Checking /spiffs/cdi.xml...
[CDI] File /spiffs/cdi.xml appears up-to-date (len 987 vs 987)
[CDI] Registering CDI with stack...
Initializing OpenLCB configuration...

Starting OpenLCB stack...
Starting executor thread...
Starting TCP Hub on port 12021...
TCP Hub listening. JMRI can connect to this device on port 12021

OpenLCB node initialization complete!
Entering run mode - will alternate events every 1 second

Allocating new alias C41 for node 050201020200
Produced event: 0x0502010202000001 (state: 1)
Produced event: 0x0502010202000000 (state: 0)
Produced event: 0x0502010202000001 (state: 1)
...
```

**Important**: Notice there is **no console message when the reset is triggered**. The reset happens silently from an external trigger. The boot diagnostics (ROM messages about `rst:0xc`, `clock div`, etc.) indicate a software reset occurred. Then the normal initialization proceeds.

After factory reset, your config file is re-initialized with defaults:
- **SNIP User Name**: "async_blink" (from `SNIP_NODE_NAME` constant)
- **SNIP User Desc**: "ESP32 Blink demo" (from `SNIP_USER_DESCRIPTION` constant)
- Any custom node name set via JMRI is **lost**

## What Happens After Factory Reset

After factory reset, subsequent boots proceed normally. The config file already exists and is valid, so no factory reset runs again unless:

- The config file becomes corrupted
- The `CANONICAL_VERSION` in firmware differs from the saved version
- You explicitly trigger another reset via JMRI Configure dialog

## Key Takeaway

`factory_reset()` runs on first boot or when triggered by JMRI, version mismatch, or config corruption to initialize a valid config file with SNIP defaults. After that, the config persists across every reboot. JMRI can edit the user name, which updates the config file while preserving other settings.
