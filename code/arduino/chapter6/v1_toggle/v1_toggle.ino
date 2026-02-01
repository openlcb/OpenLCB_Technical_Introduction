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
#include <SPIFFS.h>
#include <OpenMRNLite.h>
#include "utils/GpioInitializer.hxx"
#include "config.h"

// OpenLCB Node ID - must be unique on your network
// This ID is in the reserved range for experimental use
static constexpr uint64_t NODE_ID = 0x050201020200ULL;

// Event IDs that will be alternated
// These match the desktop async_blink example
static const uint64_t EVENT_ID_0 = 0x0502010202000000ULL;
static const uint64_t EVENT_ID_1 = 0x0502010202000001ULL;

/// This is the ESP32 pin connected to the SN65HVD23x/MCP2551 R (RX) pin.
constexpr gpio_num_t CAN_RX_PIN = GPIO_NUM_4;

/// This is the ESP32 pin connected to the SN65HVD23x/MCP2551 D (TX) pin.
constexpr gpio_num_t CAN_TX_PIN = GPIO_NUM_5;

// Create the OpenMRN stack object
OpenMRN openmrn(NODE_ID);

Esp32Can can_driver(CAN_TX_PIN, CAN_RX_PIN);

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

// Configuration variables - cached from configuration
bool blink_enabled = true;           // Default enabled, will be read from config in setup()
unsigned long last_event_time = 0;
unsigned long event_interval = 1000;  // Default, will be read from config

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

        // Read configuration values from config file and update global variables
        blink_enabled = cfg.seg().blink_enabled().read(fd);
        event_interval = cfg.seg().blink_interval().read(fd);
        Serial.printf("Configuration updated: blink_enabled = %s, blink_interval = %lu ms\n",
                      blink_enabled ? "Enabled" : "Disabled", event_interval);

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

        cfg.seg().blink_enabled().write(fd, 1);      // Default enabled
        cfg.seg().blink_interval().write(fd, 1000);  // Default 1 second
        Serial.println("Factory reset: wrote defaults (blink_enabled=Enabled, blink_interval=1000 ms)");
    }
} factory_reset_helper;

/**
 * Initialize Serial communication and print startup banner.
 */
void init_serial() {
  Serial.begin(115200);
  delay(3000);  // Give serial time to initialize
  
  Serial.println("\n\n=== OpenLCB async_blink ESP32 Example ===");
  Serial.printf("Node ID: 0x%012llX\n", NODE_ID);
  Serial.printf("Event 0: 0x%016llX\n", EVENT_ID_0);
  Serial.printf("Event 1: 0x%016llX\n", EVENT_ID_1);
  Serial.flush();
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
  can_driver.begin();
  init_openlcb_stack();
  openmrn.add_can_port(&can_driver);
  
  Serial.println("OpenLCB node initialization complete!");
  Serial.printf("Entering run mode - will alternate events every %lu ms\n", event_interval);
  
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
  
  // Check if blinking is enabled and if it's time to produce an event
  // blink_enabled and event_interval are read from config and updated in apply_configuration()
  unsigned long current_time = millis();
  
  if (blink_enabled && current_time - last_event_time >= event_interval) {
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
