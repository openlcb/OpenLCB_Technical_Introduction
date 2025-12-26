## Code Implementation

### The Complete Main Code

Now replace the contents of `src/main.cpp` with the following code:

```cpp
/** \copyright
 * Copyright (c) 2024, OpenLCB Technical Introduction
 * All rights reserved.
 *
 * Example code for educational purposes demonstrating OpenLCB node startup
 * and event handling on ESP32 using WiFi/TCP transport.
 *
 * \file main.cpp
 *
 * Simple async_blink example for ESP32 with WiFi - produces two alternating
 * events every second, demonstrating OpenLCB node initialization and event
 * production without requiring physical GPIO hardware.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <OpenMRNLite.h>
#include "utils/GcTcpHub.hxx"

#include "config.h"

// WiFi credentials - CHANGE THESE to match your network
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// OpenLCB Node ID - must be unique on your network
// This ID is in the reserved range for experimental use
static constexpr uint64_t NODE_ID = 0x050201020200ULL;

// Event IDs that will be alternated
// These match the desktop async_blink example
static const uint64_t EVENT_ID_0 = 0x0502010202000000ULL;
static const uint64_t EVENT_ID_1 = 0x0502010202000001ULL;

// Create the OpenMRN stack object
OpenMRN openmrn(NODE_ID);

// TCP Hub for JMRI connectivity
GcTcpHub* tcp_hub = nullptr;

// ConfigDef comes from config.h and defines the configuration layout
static constexpr openlcb::ConfigDef cfg(0);

// OpenLCB configuration - required by OpenMRNLite
namespace openlcb {
  // Name of CDI.xml to generate dynamically
  const char CDI_FILENAME[] = "/spiffs/cdi.xml";
  
  // This will stop openlcb from exporting the CDI memory space upon start
  const char CDI_DATA[] = "";
  
  // Path to the config file and its size
  const char* const CONFIG_FILENAME = "/spiffs/openlcb_config";
  const size_t CONFIG_FILE_SIZE = cfg.seg().size() + cfg.seg().offset();
  
  // SNIP (Simple Node Information Protocol) dynamic data storage
  const char* const SNIP_DYNAMIC_FILENAME = CONFIG_FILENAME;
}

// State variable to track which event to send
bool event_state = false;

// Timing for event production (1 second = 1000 milliseconds)
unsigned long last_event_time = 0;
const unsigned long EVENT_INTERVAL = 1000;

/**
 * Configuration update listener for factory reset and config persistence.
 * 
 * factory_reset() is called automatically by OpenMRN on first boot (when the
 * config file doesn't exist yet). It initializes SNIP dynamic data (node name
 * and description) which is then saved to SPIFFS and persists across restarts.
 * 
 * apply_configuration() is called when the user modifies configuration through
 * JMRI (or other LCC tools). In v0.1, it returns UPDATED without doing anything.
 * In Chapter 5, we'll implement actual config persistence when this is called.
 */
class FactoryResetHelper : public DefaultConfigUpdateListener
{
public:
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) OVERRIDE
    {
        AutoNotify n(done);
        // In v0.1, we don't handle runtime config changes yet.
        // Real nodes would persist changes here when the user modifies
        // configuration through JMRI. See Chapter 5 for implementation.
        return UPDATED;
    }

    void factory_reset(int fd) override
    {
        // Called on first boot to initialize the configuration file.
        // Write initial SNIP dynamic data (node name and description).
        // This data is then saved to SPIFFS and is displayed by JMRI
        // in the node properties dialog.
        cfg.userinfo().name().write(fd, openlcb::SNIP_NODE_NAME);
        cfg.userinfo().description().write(fd, openlcb::SNIP_NODE_DESC);
    }
} factory_reset_helper;

/**
 * Initialize Serial communication and print startup banner.
 */
void init_serial() {
  Serial.begin(115200);
  delay(500);  // Give serial time to initialize
  
  Serial.println("\n\n=== OpenLCB async_blink ESP32 Example ===");
  Serial.printf("Node ID: 0x%012llX\n", NODE_ID);
  Serial.printf("Event 0: 0x%016llX\n", EVENT_ID_0);
  Serial.printf("Event 1: 0x%016llX\n", EVENT_ID_1);
}

/**
 * Initialize SPIFFS filesystem for configuration storage.
 */
void init_filesystem() {
  Serial.println("\nInitializing SPIFFS...");
  if (!SPIFFS.begin(true)) {  // true = format if mount fails
    Serial.println("SPIFFS mount failed! Halting.");
    while (1) { delay(1000); }  // Stop here if filesystem fails
  }
  Serial.println("SPIFFS initialized successfully");
}

/**
 * Connect to WiFi network.
 */
void init_network() {
  Serial.printf("\nConnecting to WiFi SSID: %s\n", ssid);
  WiFi.begin(ssid, password);
  
  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected!");
  Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
}

/**
 * Initialize OpenMRN stack and configuration.
 * This creates the config file and starts the stack.
 * FactoryResetHelper automatically initializes SNIP data on first boot.
 */
void init_openlcb_stack() {
  // Create the CDI.xml dynamically
  // CDI describes what configuration options are available
  Serial.println("\nCreating CDI configuration descriptor...");
  openmrn.create_config_descriptor_xml(cfg, openlcb::CDI_FILENAME);
  
  // Create the config file if it doesn't exist
  // OpenMRNLite requires this even for simple nodes
  Serial.println("Initializing OpenLCB configuration...");
  openmrn.stack()->create_config_file_if_needed(cfg.seg().internal_config(),
                                                  openlcb::CANONICAL_VERSION,
                                                  openlcb::CONFIG_FILE_SIZE);

  // Start the OpenMRN stack
  // This initiates the OpenLCB node initialization sequence:
  // 1. Check ID (CID) - verifies Node ID is unique
  // 2. Reserve ID (RID) - claims the Node ID
  // 3. Announce Membership (AMD) - announces node to network
  // 4. Initialization Complete - node enters normal operation
  Serial.println("\nStarting OpenLCB stack...");
  openmrn.begin();
  
  // Start the executor thread for background processing
  // REQUIRED for TCP Hub to accept connections
  Serial.println("Starting executor thread...");
  openmrn.start_executor_thread();
}

/**
 * Initialize TCP Hub for JMRI connectivity.
 */
void init_tcp_hub() {
  Serial.println("Starting TCP Hub on port 12021...");
  tcp_hub = new GcTcpHub(
    openmrn.stack()->can_hub(),  // Reference to the CAN hub
    12021                        // TCP port (standard for OpenLCB)
  );
  Serial.println("TCP Hub listening. JMRI can connect to this device on port 12021");
}

/**
 * Arduino setup() - runs once at startup
 * 
 * This function initializes all hardware and software subsystems:
 * 1. Serial communication
 * 2. SPIFFS filesystem
 * 3. WiFi network
 * 4. OpenMRN stack
 * 5. TCP Hub for JMRI connectivity
 */
void setup() {
  init_serial();
  init_filesystem();
  init_network();
  init_openlcb_stack();
  init_tcp_hub();
  
  Serial.println("OpenLCB node initialization complete!");
  Serial.println("Entering run mode - will alternate events every 1 second\n");
  
  // Record start time for event production
  last_event_time = millis();
}

/**
 * Arduino loop() - runs continuously
 * 
 * This function:
 * 1. Calls openmrn.loop() to process OpenLCB protocol messages
 * 2. Alternates between two events every second
 * 3. Prints event production to serial monitor
 */
void loop() {
  // CRITICAL: Must call openmrn.loop() frequently to process messages
  openmrn.loop();
  
  // Check if it's time to produce an event (every 1 second)
  unsigned long current_time = millis();
  if (current_time - last_event_time >= EVENT_INTERVAL) {
    // Alternate event state
    event_state = !event_state;
    
    // Send the event
    uint64_t event_to_send = event_state ? EVENT_ID_1 : EVENT_ID_0;
    openmrn.stack()->executor()->add(new CallbackExecutable([event_to_send]() {
      openmrn.stack()->send_event(event_to_send);
    }));
    
    // Print to serial monitor
    Serial.printf("Produced event: 0x%016llX (state: %d)\n", 
                  event_to_send, event_state ? 1 : 0);
    
    // Update timing
    last_event_time = current_time;
  }
}
```

### Before You Build

**Update WiFi credentials in the code!** The build will succeed even with placeholder credentials, but the ESP32 won't connect to WiFi when you upload it.

> **Important**: Never check WiFi credentials (or any secrets) into a version control repository. In a real project, use environment variables or a separate configuration file excluded from git.

> **Better approach**: OpenMRNLite provides `Esp32WiFiManager` for a more robust WiFi configuration system, but we're using hardcoded credentials here for simplicity in this learning example.

> **Future note**: WiFi is temporary for learning. When we add CAN transport in a later chapter, we'll remove the WiFi code entirely. This approach helps you understand the differences between transport types.

## Making These Settings Configurable

The code above uses hardcoded values for the event interval (1 second) and SNIP user data (node name and description). In Chapter 4, you'll learn how to make these settings configurable via JMRI without recompiling the firmware.

See Chapter 4's **"Adding a Configurable Interval Setting"** section to extend this example with runtime-editable parameters, and **"Understanding Configuration Updates and Versioning"** to understand the mechanisms that make configuration reliable.


