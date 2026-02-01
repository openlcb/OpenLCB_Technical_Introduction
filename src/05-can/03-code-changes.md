# Code Changes: From WiFi to CAN

The good news: **your OpenLCB application logic doesn't change**. The event production code, configuration, and SNIP data are all transport-independent. What changes is minimal—just how the node connects to the network.

## Changes in Key Code Sections

> **Note for Arduino IDE and Arduino Maker Workshop users**: If you're following this guide using the Arduino IDE or Arduino Maker Workshop (which uses the Arduino CLI), the class names in these examples may differ. These tools can access newer Arduino Core versions that use different class names and namespacing. See the next section ("The Esp32HardwareTwai Class: Arduino Cores and the Name Change") for details on what adjustments are needed for your environment.

### Includes and Definitions (`main.cpp`)

```diff
#include <Arduino.h>
-#include <WiFi.h>
#include <SPIFFS.h>
#include <OpenMRNLite.h>
-#include "utils/GcTcpHub.hxx"
+#include "utils/GpioInitializer.hxx"
#include "config.h"

-// WiFi credentials - CHANGE THESE to match your network
-const char* ssid = "YourWiFiSSID";
-const char* password = "YourWiFiPassword";

 // OpenLCB Node ID - must be unique on your network
 static constexpr uint64_t NODE_ID = 0x050201020200ULL;

 // Event IDs that will be alternated
 static const uint64_t EVENT_ID_0 = 0x0502010202000000ULL;
 static const uint64_t EVENT_ID_1 = 0x0502010202000001ULL;

+/// This is the ESP32 pin connected to the SN65HVD23x/MCP2551 R (RX) pin.
+constexpr gpio_num_t CAN_RX_PIN = GPIO_NUM_4;
+
+/// This is the ESP32 pin connected to the SN65HVD23x/MCP2551 D (TX) pin.
+constexpr gpio_num_t CAN_TX_PIN = GPIO_NUM_5;
+
 // Create the OpenMRN stack object
 OpenMRN openmrn(NODE_ID);

-// TCP Hub for JMRI connectivity
-GcTcpHub* tcp_hub = nullptr;
+Esp32HardwareTwai can_driver(CAN_RX_PIN, CAN_TX_PIN);
```

### The `setup()` Function

```diff
void setup() {
  init_serial();
  init_filesystem();
- init_network();
+ can_driver.hw_init();
  init_openlcb_stack();
- init_tcp_hub();
+ openmrn.add_can_port_select("/dev/twai/twai0");
  
  Serial.println("OpenLCB node initialization complete!");
  Serial.printf("Entering run mode - will alternate events every %lu ms\n", event_interval);
  
  // Record start time for event production
  last_event_time = millis();
}
```

## Understanding the Changes

### 1. **Removed WiFi Includes and Code**

- `#include <WiFi.h>` — no longer needed for network connectivity
- `#include "utils/GcTcpHub.hxx"` — no longer needed (no TCP Hub)
- WiFi SSID/password variables — replaced by CAN GPIO pin definitions
- TCP Hub global variable — removed entirely

### 1.5 **Removed Functions Entirely**

Two complete functions are deleted:

**`init_network()`** — This entire function is removed:
```cpp
void init_network() {
  Serial.printf("\nConnecting to WiFi SSID: %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
}
```

**`init_tcp_hub()`** — This entire function is removed:
```cpp
void init_tcp_hub() {
  Serial.println("Starting TCP Hub on port 12021...");
  tcp_hub = new GcTcpHub(openmrn.stack()->can_hub(), 12021);
  Serial.println("TCP Hub listening. JMRI can connect to this device on port 12021");
}
```

Both are replaced by a single line in `setup()`:

```cpp
openmrn.add_can_port_select("/dev/twai/twai0");
```

### 2. **Added CAN Transceiver Definition**

The `Esp32HardwareTwai` class represents your CAN transceiver on the breadboard. It takes two GPIO pins (RX and TX) as parameters:
- **GPIO_NUM_4** (RX) — receives CAN messages
- **GPIO_NUM_5** (TX) — transmits CAN messages

### 3. **Initialization Order**

The key difference: **`can_driver.hw_init()`** initializes the ESP32's TWAI hardware and sets up GPIO4/GPIO5 for CAN signaling. This must happen *before* starting the OpenMRN stack.

### 4. **Port Registration**

Instead of creating a TCP Hub for JMRI connectivity, you register the TWAI device as a CAN port:

- `add_can_port_select("/dev/twai/twai0")` — registers the TWAI device
- `"/dev/twai/twai0"` — the filesystem device name for TWAI hardware
- `select` means the driver uses Unix `select()` for efficient I/O

The OpenMRN stack reads/writes CAN messages through this device, routing them to and from your OpenLCB application.

## Complete Updated `setup()` Function

Here's the full, updated setup function:

```cpp
void setup() {
  init_serial();
  init_filesystem();
  
  // Initialize CAN transceiver hardware
  can_driver.hw_init();
  
  // Initialize OpenMRN stack and configuration
  init_openlcb_stack();
  
  // Register the TWAI CAN device as the network port
  openmrn.add_can_port_select("/dev/twai/twai0");
  
  Serial.println("OpenLCB node initialization complete!");
  Serial.printf("Entering run mode - will alternate events every %lu ms\n", event_interval);
  
  // Record start time for event production
  last_event_time = millis();
}
```

## What Stays the Same

The following parts of your code are **unchanged**:

- **Event production logic** in `loop()` — still produces EVENT_ID_0 and EVENT_ID_1
- **Configuration structure** (config.h) — blink interval is now **configurable** via JMRI
- **SNIP data** (node name, description) — initialized on factory reset
- **FactoryResetHelper** — now also initializes default blink_interval (1000 ms)
- **apply_configuration()** — now reads the blink_interval from config when JMRI updates settings
- **openmrn.loop()** — still required in the main loop

## Key Additions: Configuration Management

This CAN version adds **runtime configuration** for the blink interval:

1. **Global variable** `event_interval` — cached value read from configuration
2. **FactoryResetHelper.apply_configuration()** — now reads `cfg.seg().blink_interval()` when configuration changes
3. **FactoryResetHelper.factory_reset()** — initializes default blink_interval to 1000 ms on first boot
4. **loop() function** — uses the configurable `event_interval` instead of a constant

This means users can change the blink speed through JMRI without recompiling the firmware.

The OpenLCB protocol stack is completely independent of the physical transport layer. This is excellent software architecture—change WiFi to CAN, and the node behaves identically.

## Compiling and Uploading

The code is ready to compile and upload using the same method as Chapter 3: click **Upload and Monitor** in the PlatformIO project tasks.

When the upload completes and the device boots, you'll see the same startup messages as before:

```
Node ID: 0x050201020200
Event 0: 0x0502010202000000
Event 1: 0x0502010202000001

Initializing SPIFFS...
SPIFFS initialized successfully
ESP-TWAI: Configuring TWAI (TX:5, RX:4, EXT-CLK:-1, BUS-CTRL:-1)

Creating CDI configuration descriptor...
ESP-TWAI: Starting TWAI watchdog and reporting task
[CDI] Checking /spiffs/cdi.xml...
[CDI] File /spiffs/cdi.xml appears up-to-date (len 1172 vs 1172)
[CDI] Registering CDI with stack...
Initializing OpenLCB configuration...

Starting OpenLCB stack...
Starting executor thread...
OpenLCB stack initialized successfully
ESP-TWAI: Starting TWAI driver on:twai0 mCode:3ffb41cc (non-blocking) fd:0
onfiguration updated: blink_interval = 2000 ms
OpenLCB node initialization complete!
Entering run mode - will alternate events every 2000 ms
Allocating new alias C41 for node 050201020200
Produced event: 0x0502010202000001 (state: 1)
Produced event: 0x0502010202000000 (state: 0)...
```

The node is now producing events on the **CAN bus** instead of the WiFi TCP Hub.

**Next**: Understand the Esp32HardwareTwai class and why it's named that.
