## Code Walkthrough

This code is organized into:
1. **Configuration** (config.h): Node identity and CDI structure
2. **Initialization** (setup function with helpers): WiFi, SPIFFS, OpenLCB stack, TCP Hub  
3. **Event production** (loop function): Alternate between two events every second

The code includes detailed comments explaining each section. We'll walk through the key concepts below.

### 1. Includes and WiFi Configuration

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <OpenMRNLite.h>

#include "config.h"

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
```

**Required includes**:
- `Arduino.h`: Core Arduino framework
- `WiFi.h`: ESP32 WiFi library for network connectivity
- `SPIFFS.h`: ESP32 filesystem library for configuration storage
- `OpenMRNLite.h`: OpenLCB protocol stack
- `config.h`: Our configuration header with CDI definitions

**Action Required**: Replace `ssid` and `password` with your actual WiFi network credentials.
- ESP32 only supports **2.4GHz WiFi networks** (not 5GHz)
- SSID is case-sensitive
- This is hardcoded for simplicity - production code would use configuration storage

### 2. Node and Event IDs

```cpp
static constexpr uint64_t NODE_ID = 0x050201020200ULL;
static const uint64_t EVENT_ID_0 = 0x0502010202000000ULL;
static const uint64_t EVENT_ID_1 = 0x0502010202000001ULL;
```

**Node ID**: Every OpenLCB node must have a globally unique 48-bit identifier. This ID (`0x050201020200`) is in the experimental range - safe for learning but not for production deployment.

**Event IDs**: These 64-bit identifiers represent the two events our node will produce. Notice they differ only in the last byte (`00` vs `01`), making them easy to track. These match the desktop `async_blink` OpenMRN example for consistency.

### 3. Configuration and OpenMRN Stack

```cpp
OpenMRN openmrn(NODE_ID);
static constexpr openlcb::ConfigDef cfg(0);

namespace openlcb {
  const char CDI_FILENAME[] = "/spiffs/cdi.xml";
  const char CDI_DATA[] = "";
  const char* const CONFIG_FILENAME = "/spiffs/openlcb_config";
  const size_t CONFIG_FILE_SIZE = cfg.seg().size() + cfg.seg().offset();
  const char* const SNIP_DYNAMIC_FILENAME = CONFIG_FILENAME;
}
```

**OpenMRN stack**: Creates the entire OpenLCB protocol stack (message routing, node initialization, event handling, network transport).

**ConfigDef**: Instantiates the CDI configuration structure from config.h (already described above). The `(0)` parameter is the offset in memory.

**OpenLCB namespace constants**:
- `CDI_FILENAME`: Path where the dynamic CDI.xml file will be written (used by JMRI for configuration discovery)
- `CDI_DATA`: Empty string tells OpenMRN to generate CDI dynamically instead of using a static resource
- `CONFIG_FILENAME`: Path to the config file in SPIFFS filesystem (note `/spiffs/` prefix)
- `CONFIG_FILE_SIZE`: Calculated as the size of all configuration segments. This ensures the file is large enough for all data (SNIP + internal config + UserInfo)
- `SNIP_DYNAMIC_FILENAME`: Store SNIP data in the same file as config

### 4. FactoryResetHelper and Configuration Initialization

```cpp
class FactoryResetHelper : public DefaultConfigUpdateListener
{
public:
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        return UPDATED;
    }

    void factory_reset(int fd) override
    {
        cfg.userinfo().name().write(fd, openlcb::SNIP_NODE_NAME);
        cfg.userinfo().description().write(fd, openlcb::SNIP_NODE_DESC);
    }
} factory_reset_helper;
```

This class handles configuration lifecycle events:

**`factory_reset(int fd)`**: Called by OpenMRN automatically on first boot when the config file is created. It initializes the SNIP dynamic data using OpenMRN's built-in CDI framework:
- `cfg.userinfo().name().write()`: Writes the node name to the correct offset in the config file
- `cfg.userinfo().description().write()`: Writes the node description to the correct offset

This approach is better than manual file I/O because:
- OpenMRN handles all byte offsets and layout automatically
- Uses the same CDI structure (ConfigDef from config.h) consistently
- Less prone to errors (no manual `fseek`, `fwrite` calls)
- Foundation ready for `apply_configuration()` in future chapters when users modify config via JMRI

> **`apply_configuration()`**: Currently returns UPDATED without doing anything. In a real node, this would handle persistence when users modify configuration through JMRI. For v0.1, config is read-only.

**SNIP data**: The node name and description are displayed by JMRI in the node properties dialog, helping identify which ESP32 is which on your network.

### 5. WiFi Connection

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

This helper function establishes the WiFi connection before starting OpenLCB. The ESP32 won't be able to communicate on the OpenLCB network until WiFi is connected, so we wait here.

The dots printed to serial provide visual feedback during connection.

### 6. Initialization Helper Functions and Setup

```cpp
void init_openlcb_stack() {
  Serial.println("\nCreating CDI configuration descriptor...");
  openmrn.create_config_descriptor_xml(cfg, openlcb::CDI_FILENAME);
  
  Serial.println("Initializing OpenLCB configuration...");
  openmrn.stack()->create_config_file_if_needed(cfg.seg().internal_config(),
                                                  openlcb::CANONICAL_VERSION,
                                                  openlcb::CONFIG_FILE_SIZE);

  Serial.println("\nStarting OpenLCB stack...");
  openmrn.begin();
  
  Serial.println("Starting executor thread...");
  openmrn.start_executor_thread();
}
```

This helper function performs several critical initialization steps:

1. **Create CDI.xml dynamically**: `openmrn.create_config_descriptor_xml()` generates a CDI (Configuration Description Information) file that describes all available configuration options. JMRI uses this file to know what settings the node supports. The file is written to `/spiffs/cdi.xml`.

2. **Create config file if needed**: `create_config_file_if_needed()` ensures the config file exists with proper structure. On first boot:
   - File is created with space for internal config, SNIP data, and UserInfo
   - FactoryResetHelper's `factory_reset()` is called automatically
   - SNIP user data (name and description) is populated

3. **Start OpenMRN stack**: `openmrn.begin()` initiates the entire OpenLCB protocol sequence (CID, RID, AMD) as described in Chapter 1.

4. **Start executor thread**: Background thread for processing OpenLCB messages. **This is required for TCP Hub to work.**

### 7. TCP Hub for JMRI Connectivity

```cpp
void init_tcp_hub() {
  Serial.println("Starting TCP Hub on port 12021...");
  tcp_hub = new GcTcpHub(
    openmrn.stack()->can_hub(),
    12021
  );
  Serial.println("TCP Hub listening. JMRI can connect to this device on port 12021");
}
```

This helper function creates a TCP server on port 12021 using the GridConnect protocol (the ASCII format that JMRI expects). It's passed:
- `openmrn.stack()->can_hub()`: Reference to the message router
- `12021`: The TCP port to listen on (standard for OpenLCB TCP hubs)

The TCP Hub allows JMRI and other TCP clients to connect and monitor your node's events in real-time. Multiple JMRI instances can connect simultaneously; messages are routed between all connected clients and the local node.

### 8. Main Setup Function

```cpp
void setup() {
  init_serial();
  init_filesystem();
  init_network();
  init_openlcb_stack();
  init_tcp_hub();
  
  Serial.println("OpenLCB node initialization complete!");
  Serial.println("Entering run mode - will alternate events every 1 second\n");
  
  last_event_time = millis();
}
```

The `setup()` function calls four helper functions in sequence:

1. **`init_serial()`**: Initialize Serial, print startup banner with Node ID and Event IDs
2. **`init_filesystem()`**: Initialize SPIFFS filesystem
3. **`init_network()`**: Connect to WiFi (required before OpenLCB)
4. **`init_openlcb_stack()`**: Create CDI file, config file, initialize SNIP data via FactoryResetHelper, start OpenLCB protocol stack, start executor thread
5. **`init_tcp_hub()`**: Start TCP server for JMRI connectivity

**Why break it down?** Each helper function focuses on a single responsibility, making the code easier to understand and modify. If you need to add new initialization steps or change how the node starts up, it's clear where to make those changes.

### 9. Event Production Loop

```cpp
void loop() {
  openmrn.loop();  // CRITICAL - processes all OpenLCB messages
  
  unsigned long current_time = millis();
  if (current_time - last_event_time >= EVENT_INTERVAL) {
    event_state = !event_state;
    uint64_t event_to_send = event_state ? EVENT_ID_1 : EVENT_ID_0;
    
    openmrn.stack()->executor()->add(new CallbackExecutable([event_to_send]() {
      openmrn.stack()->send_event(event_to_send);
    }));
    
    Serial.printf("Produced event: 0x%016llX (state: %d)\n", 
                  event_to_send, event_state ? 1 : 0);
    
    last_event_time = current_time;
  }
}
```

**Critical Detail**: `openmrn.loop()` must be called frequently (ideally every few milliseconds). This processes:
- Incoming network messages
- Outgoing message queues
- Protocol state machines
- Internal timers

**Event Production**: Every 1000ms (1 second), we:
1. Toggle `event_state` (false → true → false → ...)
2. Select which event ID to send based on state
3. Queue the event for transmission using the executor
4. Print confirmation to serial monitor

**Why use the executor?** OpenLCB message handling runs in a separate execution context. The `executor()->add()` pattern ensures thread-safe event production.

### What Happens Next

Now that you understand how the code works, the next section covers building this code and uploading it to your ESP32. You'll see all of these initialization steps happen in the serial monitor, and we'll verify everything is working correctly.
