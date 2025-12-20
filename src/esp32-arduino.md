# ESP32 with Arduino & PlatformIO

This chapter covers everything needed to build and deploy an OpenLCB node on an ESP32 microcontroller using Arduino and PlatformIO. We'll create a working example that you can modify for your own applications.

## Overview

The ESP32 is a powerful, affordable microcontroller with built-in WiFi connectivity, making it ideal for learning OpenLCB concepts. In this chapter, we'll build a complete OpenLCB node that simultaneously runs the node protocol stack and hosts a TCP Hub—allowing JMRI to monitor and control your node over WiFi.

As described in the "Network Architecture" section of Chapter 2, your ESP32 will act as both:
- An **OpenLCB node** that produces and consumes events
- A **TCP Hub** listening on port 12021 for JMRI connections

We'll build this in two phases:
1. **Software-only**: Create a node that produces and consumes events via JMRI (no physical hardware needed yet)
2. **Hardware integration** (optional): Add a physical button and LED to your breadboard

This approach lets you start learning immediately while waiting for breadboard components to arrive.

## ESP32 Board Selection

Before installing software, you'll want to order an ESP32 development board. This section helps you choose the right board for this tutorial.

The ESP32 family includes many board variants. For this tutorial, we recommend boards with sufficient GPIO pins, USB programming support, and built-in CAN capability for future chapters.

### Recommended Board: ESP32 DevKit V1

**ESP32 DevKit V1 / ESP32-WROOM-32** (Xtensa architecture)
- Most common and affordable ESP32 development board
- 30+ GPIO pins available (plenty for expansion)
- Built-in USB-to-serial converter (CP2102 or CH340)
- 4MB flash memory (sufficient for OpenMRNLite applications)
- Built-in CAN controller (TWAI) for future CAN chapters
- **Best OpenMRNLite compatibility** - fully tested and supported
- Available from multiple manufacturers (Espressif, DOIT, etc.)
- Cost: $5-10 USD

This is the board we'll use throughout the tutorial.

### Alternative Boards

**ESP32-DevKitC**
- Official Espressif development board
- Similar pinout to DevKit V1
- Excellent documentation and support
- Slightly more expensive but guaranteed quality

**ESP32-S3** (Xtensa architecture)
- Newer variant with USB-OTG support
- More memory and GPIO options
- Built-in CAN controller
- Good OpenMRNLite support

### Boards to Avoid (for now)

**ESP32-C3** (RISC-V architecture)
- Different CPU architecture (RISC-V vs Xtensa)
- OpenMRNLite has compatibility issues with ESP32-C3
- Missing required ESP-IDF headers in Arduino framework
- Wait for future OpenMRNLite updates before using

### What You Need Now

To get started with the software-only version:
- **Just the ESP32 board** with USB cable

For the hardware integration phase (optional, later):
- Solderless breadboard
- Tactile pushbutton
- LED (any color)
- 220Ω resistor
- Jumper wires

### Purchasing

ESP32 boards are available from:
- **Amazon** / **eBay**: Search "ESP32 DevKit" (verify reviews)
- **AliExpress** / **Banggood**: Direct from manufacturers (longer shipping)
- **Adafruit** / **SparkFun**: Higher quality, better support, higher cost
- **DigiKey** / **Mouser**: For bulk or commercial projects

**Order your board now**, then continue with the software setup while you wait for delivery.

## PlatformIO Installation & Setup

PlatformIO is a professional embedded development platform that provides a unified build system, library management, and debugging tools. It integrates with VS Code to create a powerful development environment.

### Installing VS Code

If you don't already have Visual Studio Code installed:

1. Download VS Code from [code.visualstudio.com](https://code.visualstudio.com/)
2. Run the installer for your operating system (Windows, macOS, or Linux)
3. Follow the installation wizard with default options
4. Launch VS Code after installation completes

### Installing PlatformIO Extension

1. Open VS Code
2. Click the Extensions icon in the left sidebar (or press `Ctrl+Shift+X` / `Cmd+Shift+X`)
3. Search for "PlatformIO IDE"
4. Click **Install** on the "PlatformIO IDE" extension by PlatformIO
5. Wait for the installation to complete (this may take several minutes as it downloads toolchains)
6. Restart VS Code when prompted

After restarting, you should see a new PlatformIO icon (alien head) in the left sidebar.

> **Note**: The ESP32 platform and toolchain will be installed automatically when you create your first project. PlatformIO handles all the toolchain downloads and configuration for you.

## Creating Your First Project: async_blink_esp32

Now we'll create the actual project we'll be working with throughout this chapter. Instead of creating a throwaway test project, we'll jump straight into building our OpenLCB node.

### Create the Project

1. Click the PlatformIO icon in the left sidebar
2. Select **New Project** from Quick Access
3. Enter project name: **async_blink_esp32**
4. For Board, search and select **DOIT ESP32 DEVKIT V1** (or `esp32doit-devkit-v1`)
5. Framework should automatically select **Arduino**
6. Click **Finish**

PlatformIO will:
- Create the project structure
- Download the ESP32 platform and toolchain (first time only, may take several minutes)
- Set up the Arduino framework
- Create a basic `src/main.cpp` file

### Understanding the Project Structure

After creation, you'll see this structure:

```
async_blink_esp32/
├── platformio.ini     # Project configuration
├── src/
│   └── main.cpp       # Your application code (we'll replace this)
├── lib/               # Project-specific libraries
├── include/           # Header files
└── test/              # Unit tests (optional)
```

The `platformio.ini` file should look like this:

```ini
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
```

**Note**: The `monitor_speed = 115200` setting configures the serial monitor baud rate to match what our code will use. Without this, the serial output will appear as garbled characters.

## Adding OpenMRNLite to Your Project

OpenMRNLite is the lightweight version of OpenMRN designed for Arduino-compatible platforms. It provides all the core LCC/OpenLCB functionality without the full complexity of the OpenMRN framework.

### Installation via platformio.ini

Open the `platformio.ini` file in your `async_blink_esp32` project and add OpenMRNLite to the library dependencies:

```ini
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
lib_deps = openmrn/OpenMRNLite@2.0.0
monitor_speed = 115200
```

Save the file. That's it! PlatformIO will automatically download OpenMRNLite from the registry when you build the project.

**About monitor_speed**: This setting ensures the serial monitor uses 115200 baud, matching the `Serial.begin(115200)` call in our code. Without this, you'll see garbled output instead of readable text.

**Why version 2.0.0?**
- Version 2.0.0 is fully compatible with PlatformIO's current ESP32 platform
- Later versions (2.2.x+) require newer ESP-IDF features not yet available in PlatformIO
- All core OpenLCB functionality is present in v2.0.0

> **About PlatformIO and Arduino-ESP32 versions**
>
> You may notice that PlatformIO uses Arduino-ESP32 v2.0.17 while the Arduino IDE can install v3.x. This isn't a bug — it's intentional:
>
> - **PlatformIO prioritizes stability** over bleeding-edge features. They thoroughly test each framework version across hundreds of boards, toolchains, and debuggers before releasing.
> - **Arduino-ESP32 3.x introduced major breaking changes** in the build system, partition handling, WiFi stack, and more. PlatformIO waits for these to stabilize before adoption.
> - **Small team, massive ecosystem**: PlatformIO maintains ESP32, STM32, RP2040, AVR, and many other platforms with a much smaller team than Espressif has for Arduino-ESP32 alone.
>
> This is why we use OpenMRNLite v2.0.0 — it's designed for the stable Arduino-ESP32 2.x that PlatformIO provides. You're trading cutting-edge features for rock-solid reliability, which is perfect for learning OpenLCB fundamentals.

### Verification

Let's verify everything is working by building the project:

1. In VS Code, click the PlatformIO icon in the left sidebar
2. Under **PROJECT TASKS** → **esp32doit-devkit-v1**, click **Build**
3. PlatformIO will:
   - Download OpenMRNLite (first time only)
   - Compile the default `main.cpp`
   - Display build output

4. Look for **SUCCESS** at the end of the output

> **Note**: The first build will take longer as PlatformIO downloads the library and compiles it. Subsequent builds are much faster.

If the build succeeds, OpenMRNLite is installed correctly and you're ready to write code!

## Writing the async_blink_esp32 Code

Now we'll create the actual OpenLCB node that produces alternating events. This demonstrates the core OpenLCB protocol behavior you learned in Chapter 1 (node initialization and event production) without requiring physical hardware.

### Creating the Configuration Header

OpenMRNLite requires a configuration structure (CDI - Configuration Description Information) even for simple nodes. We'll create a minimal config.h file.

Create the file `include/config.h` with this content:

```cpp
#ifndef _ASYNC_BLINK_CONFIG_H_
#define _ASYNC_BLINK_CONFIG_H_

#include "openlcb/ConfigRepresentation.hxx"
#include "openlcb/MemoryConfig.hxx"

namespace openlcb {

/// Node identification information
extern const SimpleNodeStaticValues SNIP_STATIC_DATA = {
    4,               // Version
    "OpenMRN",       // Manufacturer
    "async_blink",   // Model
    "ESP32",         // Hardware version
    "1.00"           // Software version
};

/// Version number for the configuration structure
static constexpr uint16_t CANONICAL_VERSION = 0x0001;

/// Minimal configuration segment with just internal config
CDI_GROUP(AsyncBlinkSegment, Segment(MemoryConfigDefs::SPACE_CONFIG), Offset(128));
CDI_GROUP_ENTRY(internal_config, InternalConfigData);
CDI_GROUP_END();

/// The main CDI structure
CDI_GROUP(ConfigDef, MainCdi());
CDI_GROUP_ENTRY(ident, Identification);
CDI_GROUP_ENTRY(seg, AsyncBlinkSegment);
CDI_GROUP_END();

} // namespace openlcb

#endif // _ASYNC_BLINK_CONFIG_H_
```

**What this does**: Defines the minimal CDI structure OpenMRNLite needs. The `SNIP_STATIC_DATA` provides node identification (manufacturer, model, version), and `ConfigDef` defines the configuration memory layout. Even though we don't use configuration in this simple example, OpenMRNLite requires these structures to exist.

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
  
  // Path to the CDI (Configuration Description Information) file
  const char CDI_FILENAME[] = "/spiffs/cdi.xml";
  
  // This disables CDI exports - we don't need configuration for this simple example
  extern const char CDI_DATA[] = "";
  
  // Path to the config file and its size
  const char* const CONFIG_FILENAME = "/spiffs/openlcb_config";
  const size_t CONFIG_FILE_SIZE = 256;
  
  // SNIP (Simple Node Information Protocol) dynamic data storage
  const char* const SNIP_DYNAMIC_FILENAME = CONFIG_FILENAME;
}

// State variable to track which event to send
bool event_state = false;

// Timing for event production (1 second = 1000 milliseconds)
unsigned long last_event_time = 0;
const unsigned long EVENT_INTERVAL = 1000;

/**
 * Arduino setup() - runs once at startup
 * 
 * This function:
 * 1. Initializes serial communication for debugging
 * 2. Initializes SPIFFS filesystem
 * 3. Connects to WiFi
 * 4. Creates config file if needed
 * 5. Starts the OpenMRN stack
 * 6. Initiates OpenLCB node initialization sequence (CID, RID, AMD)
 */
void setup() {
  // Initialize Serial for debugging output
  Serial.begin(115200);
  delay(500);  // Give serial time to initialize
  
  Serial.println("\n\n=== OpenLCB async_blink ESP32 Example ===");
  Serial.printf("Node ID: 0x%012llX\n", NODE_ID);
  Serial.printf("Event 0: 0x%016llX\n", EVENT_ID_0);
  Serial.printf("Event 1: 0x%016llX\n", EVENT_ID_1);
  
  // Initialize SPIFFS filesystem for configuration storage
  Serial.println("\nInitializing SPIFFS...");
  if (!SPIFFS.begin(true)) {  // true = format if mount fails
    Serial.println("SPIFFS mount failed! Halting.");
    while (1) { delay(1000); }  // Stop here if filesystem fails
  }
  Serial.println("SPIFFS initialized successfully");
  
  // Connect to WiFi
  Serial.printf("\nConnecting to WiFi SSID: %s\n", ssid);
  WiFi.begin(ssid, password);
  
  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected!");
  Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
  
  // Create the config file if it doesn't exist
  // OpenMRNLite requires this even for simple nodes
  Serial.println("\nInitializing OpenLCB configuration...");
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
  
  // Create and start the TCP Hub
  // This allows JMRI and other TCP clients to connect
  Serial.println("Starting TCP Hub on port 12021...");
  tcp_hub = new GcTcpHub(
    openmrn.stack()->can_hub(),  // Reference to the CAN hub
    12021                        // TCP port (standard for OpenLCB)
  );
  Serial.println("TCP Hub listening. JMRI can connect to this device on port 12021");
  
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

### Code Walkthrough

Let's break down each section to understand how it works.

#### 1. Includes and WiFi Configuration

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

#### 2. Node and Event IDs

```cpp
static constexpr uint64_t NODE_ID = 0x050201020200ULL;
static const uint64_t EVENT_ID_0 = 0x0502010202000000ULL;
static const uint64_t EVENT_ID_1 = 0x0502010202000001ULL;
```

**Node ID**: Every OpenLCB node must have a globally unique 48-bit identifier. This ID (`0x050201020200`) is in the experimental range - safe for learning but not for production deployment.

**Event IDs**: These 64-bit identifiers represent the two events our node will produce. Notice they differ only in the last byte (`00` vs `01`), making them easy to track. These match the desktop `async_blink` OpenMRN example for consistency.

#### 3. Configuration and OpenMRN Stack

```cpp
OpenMRN openmrn(NODE_ID);
static constexpr openlcb::ConfigDef cfg(0);

namespace openlcb {
  const char CDI_FILENAME[] = "/spiffs/cdi.xml";
  extern const char CDI_DATA[] = "";
  const char* const CONFIG_FILENAME = "/spiffs/openlcb_config";
  const size_t CONFIG_FILE_SIZE = 256;
  const char* const SNIP_DYNAMIC_FILENAME = CONFIG_FILENAME;
}
```

**OpenMRN stack**: Creates the entire OpenLCB protocol stack (message routing, node initialization, event handling, network transport).

**ConfigDef**: Instantiates the configuration structure from config.h. The `(0)` parameter is the offset in memory.

**OpenLCB namespace constants**:
- `CDI_FILENAME`: Where to store the CDI XML file (we won't use it)
- `CDI_DATA`: Empty string disables CDI exports for this simple example
- `CONFIG_FILENAME`: Path to the config file in SPIFFS filesystem (note `/spiffs/` prefix)
- `CONFIG_FILE_SIZE`: 256 bytes is minimal for a simple node
- `SNIP_DYNAMIC_FILENAME`: Store SNIP data in the same file as config

#### 4. SPIFFS Filesystem Initialization (setup function)

```cpp
Serial.println("\nInitializing SPIFFS...");
if (!SPIFFS.begin(true)) {
  Serial.println("SPIFFS mount failed! Halting.");
  while (1) { delay(1000); }
}
Serial.println("SPIFFS initialized successfully");
```

**SPIFFS** (SPI Flash File System) is the ESP32's built-in filesystem. OpenMRNLite needs it to store configuration data persistently.

**`SPIFFS.begin(true)`**: The `true` parameter means "format if mount fails" - on first run, it automatically formats the filesystem.

**Error handling**: If SPIFFS fails to mount even after formatting, we halt in an infinite loop. This prevents the node from starting without proper storage.

#### 5. WiFi Connection

```cpp
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
```

This establishes the WiFi connection before starting OpenLCB. The ESP32 won't be able to communicate on the OpenLCB network until WiFi is connected, so we wait here.

The dots printed to serial provide visual feedback during connection.

#### 6. Configuration File Creation

```cpp
Serial.println("\nInitializing OpenLCB configuration...");
openmrn.stack()->create_config_file_if_needed(cfg.seg().internal_config(),
                                              openlcb::CANONICAL_VERSION,
                                              openlcb::CONFIG_FILE_SIZE);
```

This creates `/spiffs/openlcb_config` if it doesn't already exist. The file stores:
- Internal configuration data (version marker, etc.)
- SNIP dynamic data (node name, description)

**Why this is needed**: OpenMRNLite's configuration system expects this file to exist before `openmrn.begin()` is called. The `create_config_file_if_needed()` method handles creating and initializing it with the proper structure.

#### 7. OpenLCB Stack Startup

```cpp
openmrn.begin();
```

This single call initiates the entire OpenLCB node initialization sequence:

1. **Check ID (CID)**: Broadcasts the Node ID on the network to see if anyone else is using it
2. **Reserve ID (RID)**: If no conflicts, claims the Node ID
3. **Announce Membership (AMD)**: Tells other nodes this node is now active
4. **Initialization Complete**: Node enters normal operation mode

This matches the sequence diagram from Chapter 1. All of this happens inside `openmrn.begin()` - you don't need to implement it yourself!

#### 8. Starting the Executor Thread and TCP Hub

After the OpenLCB stack initializes, we need to start two essential services:

```cpp
// Start the executor thread for background processing
// REQUIRED for TCP Hub to accept connections
Serial.println("Starting executor thread...");
openmrn.start_executor_thread();

// Create and start the TCP Hub
// This allows JMRI and other TCP clients to connect
Serial.println("Starting TCP Hub on port 12021...");
tcp_hub = new GcTcpHub(
  openmrn.stack()->can_hub(),  // Reference to the CAN hub
  12021                        // TCP port (standard for OpenLCB)
);
Serial.println("TCP Hub listening. JMRI can connect to this device on port 12021");
```

**Why This Matters**: 

Remember from Chapter 2, your ESP32 is now both a **node** (producing events) and a **hub** (accepting JMRI connections). The TCP Hub is what makes JMRI connection possible.

- **`start_executor_thread()`**: OpenLCB uses background tasks to process network messages. The executor thread runs these tasks. **This is required for the TCP Hub to work.**
  
- **`GcTcpHub`**: Creates a TCP server on port 12021 using the GridConnect protocol (the ASCII format that JMRI expects). It's passed:
  - `openmrn.stack()->can_hub()`: Reference to the message router
  - `12021`: The TCP port to listen on (standard for OpenLCB TCP hubs)

- **Multiple Clients**: The TCP Hub can accept multiple JMRI instances connecting simultaneously. Messages are routed between all connected clients and the local node.

**In Your Code**: This code goes right after `openmrn.begin()` in the `setup()` function, before your node enters the event production loop. It's how we expose your node to JMRI for monitoring and testing.

#### 9. Event Production Loop

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

### Expected Serial Monitor Output

When the code runs, you should see output like this:

```
=== OpenLCB async_blink ESP32 Example ===
Node ID: 0x050201020200
Event 0: 0x0502010202000000
Event 1: 0x0502010202000001

Initializing SPIFFS...
SPIFFS initialized successfully

Connecting to WiFi SSID: YourNetwork
.....
WiFi connected!
IP Address: 192.168.1.100

Initializing OpenLCB configuration...
Starting OpenLCB stack...
Starting executor thread...
Starting TCP Hub on port 12021...
TCP Hub listening. JMRI can connect to this device on port 12021

OpenLCB node initialization complete!
Entering run mode - will alternate events every 1 second

Produced event: 0x0502010202000000 (state: 0)
Produced event: 0x0502010202000001 (state: 1)
Produced event: 0x0502010202000000 (state: 0)
Produced event: 0x0502010202000001 (state: 1)
...
```

This confirms:
- SPIFFS filesystem initialized
- WiFi connection succeeded
- OpenLCB node initialized
- **TCP Hub is listening on port 12021** - JMRI can now connect!
- Events are being produced alternately

### Before You Build

**Update WiFi credentials in the code!** The build will succeed even with placeholder credentials, but the ESP32 won't connect to WiFi when you upload it.

## Building and Uploading to ESP32

Now that you have the complete code, let's build it, upload it to your ESP32, and verify it works through the serial monitor.

### Building the Project

1. **Save the modified main.cpp file** (`Ctrl+S` / `Cmd+S`)

2. **Open the PlatformIO sidebar**:
   - Click the PlatformIO icon (alien head) in VS Code's left sidebar
   - Or use the bottom toolbar's checkmark icon (Build)

3. **Build the project**:
   - In PROJECT TASKS → **esp32doit-devkit-v1**, click **Build**
   - Or click the checkmark (✓) icon in the bottom toolbar
   - Or press `Ctrl+Alt+B` / `Cmd+Alt+B`

4. **Watch the build output**:
   ```
   Building in release mode
   Compiling .pio/build/esp32doit-devkit-v1/src/main.cpp.o
   Linking .pio/build/esp32doit-devkit-v1/firmware.elf
   Building .pio/build/esp32doit-devkit-v1/firmware.bin
   ========================= [SUCCESS] Took 5.23 seconds =========================
   ```

The first build takes longer because it compiles OpenMRNLite. Subsequent builds are much faster.

**If the build fails**, check:
- WiFi credentials are properly quoted (strings)
- All braces `{}` and parentheses `()` match
- `#include <OpenMRNLite.h>` is present
- `lib_deps` in `platformio.ini` includes OpenMRNLite

### Connecting Your ESP32

1. **Connect the ESP32 to your computer** via USB cable
   - Use a **data cable**, not a charge-only cable
   - The ESP32 should power on (onboard LED may light up)

2. **Identify the COM port** (Windows) or device path (Mac/Linux):
   - PlatformIO usually auto-detects the port
   - Windows: `COM3`, `COM4`, etc.
   - Mac: `/dev/cu.usbserial-*` or `/dev/cu.wchusbserial*`
   - Linux: `/dev/ttyUSB0` or `/dev/ttyACM0`

3. **If the port isn't detected**, you may need to install a USB driver:
   - **CP2102**: Download from [Silicon Labs](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
   - **CH340**: Download from [WCH](http://www.wch-ic.com/downloads/CH341SER_EXE.html)
   - Most modern operating systems include these drivers

### Uploading the Firmware

1. **Upload the firmware**:
   - In PROJECT TASKS → **esp32doit-devkit-v1**, click **Upload**
   - Or click the arrow (→) icon in the bottom toolbar
   - Or press `Ctrl+Alt+U` / `Cmd+Alt+U`

2. **Watch the upload process**:
   ```
   Configuring upload protocol...
   Looking for upload port...
   Auto-detected: COM3
   Uploading .pio/build/esp32doit-devkit-v1/firmware.bin
   esptool.py v4.5.1
   Connecting........__
   Chip is ESP32-D0WDQ6 (revision 1)
   Writing at 0x00010000... (100 %)
   Wrote 876544 bytes (543210 compressed) at 0x00010000 in 48.2 seconds
   Leaving...
   Hard resetting via RTS pin...
   ========================= [SUCCESS] Took 52.91 seconds =========================
   ```

3. **The ESP32 will automatically reboot** and start running your code

**If upload fails**:
- **"Serial port not found"**: Check USB cable connection, try different USB port
- **"Failed to connect"**: Hold the BOOT button while clicking upload, release after "Connecting..." appears
- **Permission denied (Linux)**: Add your user to the `dialout` group: `sudo usermod -a -G dialout $USER`, then log out and back in

### Opening the Serial Monitor

The serial monitor lets you see the debug output from your ESP32 in real-time.

1. **Open the serial monitor**:
   - In PROJECT TASKS → **esp32doit-devkit-v1**, click **Monitor**
   - Or click the plug icon (🔌) in the bottom toolbar
   - Or press `Ctrl+Alt+S` / `Cmd+Alt+S`

2. **The serial monitor opens** in a new terminal panel at the bottom of VS Code

3. **If you missed the startup sequence**, press the **RESET** button on your ESP32 board to restart it

### Verifying the Output

You should see output similar to this:

```
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

Initializing OpenLCB configuration...

Starting OpenLCB stack...
OpenLCB node initialization complete!
Entering run mode - will alternate events every 1 second

Produced event: 0x0502010202000000 (state: 0)
Produced event: 0x0502010202000001 (state: 1)
Produced event: 0x0502010202000000 (state: 0)
Produced event: 0x0502010202000001 (state: 1)
...
```

**What to verify**:
- ✅ SPIFFS initializes successfully
- ✅ WiFi connects successfully (shows your network name and IP address)
- ✅ OpenLCB stack initializes
- ✅ Events alternate between `...00` and `...01` every second
- ✅ State toggles between `0` and `1`

### Understanding What You See

This output confirms your ESP32 is:

1. **Connected to WiFi**: The IP address shows it's on your network
2. **Running the OpenLCB stack**: Node initialization completed successfully
3. **Producing events**: The alternating event IDs prove the event production logic works
4. **Ready for network communication**: The node is broadcasting these events on the OpenLCB network (you'll verify this with JMRI next)

**Congratulations!** You have a working OpenLCB node. The events are being broadcast over WiFi/TCP, but you can't see them on the network yet - that's what JMRI will show you in the next section.

### Common Issues

**WiFi won't connect** (stuck on dots):
- Verify WiFi credentials in code are correct
- Check ESP32 is within range of your access point
- Confirm your network is 2.4GHz (not 5GHz only)
- Some corporate/school networks block device connections

**No serial output at all**:
- Verify `monitor_speed = 115200` is in your `platformio.ini` file
- Try pressing RESET button on ESP32
- Verify the serial monitor is connected to the correct port

**Output is garbled/random characters**:
- Wrong baud rate - ensure `monitor_speed = 115200` is in `platformio.ini`
- If you already added it, stop and restart the serial monitor
- Bad USB cable or connection

**"Brownout detector triggered" errors**:
- Insufficient power from USB port
- Try a different USB port or powered USB hub
- This usually doesn't prevent operation, just a warning

## Testing with JMRI

Now that your ESP32 is producing events, let's use JMRI (Java Model Railroad Interface) to monitor them on the network. JMRI acts as a "traffic monitor" that shows all OpenLCB messages, letting you verify your node is working correctly.

### What is JMRI?

JMRI is an open-source application suite for model railroading that includes comprehensive OpenLCB/LCC support. For our purposes, we'll use it to:
- Monitor all OpenLCB messages on the network
- See when nodes initialize (CID, RID, AMD messages)
- Observe event production and consumption
- Verify your ESP32 is communicating correctly

Think of it as a "network packet sniffer" for OpenLCB.

### Installing JMRI

1. **Download JMRI**:
   - Visit [jmri.org](https://www.jmri.org/)
   - Click **Download** → Latest Production Release
   - Choose your operating system (Windows, macOS, or Linux)
   - Download and run the installer

2. **System Requirements**:
   - Java 11 or newer (usually bundled with JMRI installer)
   - Windows 7+, macOS 10.14+, or Linux with X11
   - ~500MB disk space

3. **Install JMRI**:
   - Run the installer with default options
   - On macOS, you may need to allow the app in System Preferences → Security
   - On Linux, you may need to make the script executable: `chmod +x JMRI-installer.sh`

4. **Launch JMRI**:
   - Start **DecoderPro** or **PanelPro** (either works - they share the same connection system)
   - First launch may take 30-60 seconds as JMRI initializes

### Configuring the OpenLCB Connection

JMRI needs to know how to connect to the OpenLCB network. As discussed in Chapter 2 ("Network Architecture"), your ESP32 runs a TCP Hub on port 12021 that JMRI will connect to. Let's configure that connection.

1. **Open Preferences**:
   - In JMRI, go to **Edit → Preferences** (Windows/Linux) or **JMRI → Preferences** (macOS)

2. **Add a new connection**:
   - Click the **Connections** tab (in the left sidebar)
   - Click the **+** button (bottom left) to add a new connection

3. **Configure the connection**:
   - **System manufacturer**: Select `OpenLCB`
   - **System connection**: Select `CAN via GridConnect Network Interface`
   - **Connection name**: Enter `ESP32 async_blink` (or any descriptive name)
   - **IP Address/Host Name**: Enter the IP address your ESP32 showed in serial monitor (e.g., `192.168.1.100`)
   - **TCP/UDP Port**: Enter `12021`
   - **Connection Protocol**: Select `TCP`

4. **Save settings**:
   - Click **Save** at the bottom of the Preferences window
   - JMRI will prompt to restart - click **Restart Now**

### Monitoring OpenLCB Traffic

After JMRI restarts, let's open the message monitor to see network traffic.

1. **Open the OpenLCB Monitor**:
   - Go to **OpenLCB → Monitor Traffic**
   - A new window opens showing a live feed of OpenLCB messages

2. **Look for your ESP32's startup sequence**:
   - If your ESP32 is already running, press its **RESET** button to trigger initialization
   - Watch for messages in the monitor

### Understanding the Message Traffic

You should see messages similar to this (your Node ID will differ):

```
[Check ID] 05 02 01 02 02 00
[Reserve ID] 05 02 01 02 02 00
[Announce Node] 05 02 01 02 02 00
[Producer Consumer Event Report] 05 02 01 02 02 00 00 00
[Producer Consumer Event Report] 05 02 01 02 02 00 00 01
[Producer Consumer Event Report] 05 02 01 02 02 00 00 00
[Producer Consumer Event Report] 05 02 01 02 02 00 00 01
...
```

Let's decode what you're seeing:

#### 1. Node Initialization Sequence (First 3 messages)

These correspond to the startup sequence from Chapter 1:

- **Check ID (CID)**: `05 02 01 02 02 00` - "Is anyone using this Node ID?"
- **Reserve ID (RID)**: `05 02 01 02 02 00` - "I'm claiming this Node ID"
- **Announce Node (AMD)**: `05 02 01 02 02 00` - "New node active on the network"

This happens once when the node starts (or when you press RESET).

#### 2. Event Production (Repeating messages)

After initialization, you'll see alternating event reports every second:

- **Event `...00 00 00`**: First event (state 0)
- **Event `...00 00 01`**: Second event (state 1)

These should match the timestamps in your serial monitor output!

**Verification**: Open both the JMRI monitor and your ESP32 serial monitor side-by-side. Every time the serial monitor prints `Produced event: 0x0502010202000000`, you should see a corresponding message in JMRI within ~100ms.

### What This Proves

By seeing the events in JMRI, you've confirmed:

✅ **WiFi communication works**: ESP32 is connected to the network
✅ **OpenLCB protocol is correct**: Node initialization followed proper CID/RID/AMD sequence
✅ **Event production is working**: Events are broadcast and received by other nodes (JMRI)
✅ **Timing is accurate**: Events alternate every ~1 second as expected

Your ESP32 is now a functioning OpenLCB node!

### Optional: Configure JMRI as an Event Consumer

Want to see the interaction go the other way? Let's configure JMRI to consume your ESP32's events.

1. **Create an event consumer in JMRI**:
   - Go to **Tools → Tables → Sensors**
   - Click **Add** to create a new sensor
   - **System Name**: `MS05.02.01.02.02.00.00.00` (matches EVENT_ID_0)
   - **User Name**: `ESP32 State 0`
   - Click **OK**

2. **Repeat for the second event**:
   - **System Name**: `MS05.02.01.02.02.00.00.01` (matches EVENT_ID_1)
   - **User Name**: `ESP32 State 1`

3. **Watch the sensor table** - the sensors will toggle between ACTIVE/INACTIVE as your ESP32 produces events!

This demonstrates the bidirectional nature of OpenLCB - any node can produce or consume events.

### Troubleshooting JMRI Connection

**JMRI shows "Connection failed" or "No route to host"**:
- Verify the IP address matches what the ESP32 serial monitor showed
- Ensure your computer and ESP32 are on the same WiFi network
- Check firewall settings - allow Java/JMRI to access the network
- Try pinging the ESP32: `ping 192.168.1.100` (use your ESP32's IP)

**No messages appear in the monitor**:
- Click **Clear** in the monitor to reset the display
- Press RESET on the ESP32 to trigger initialization messages
- Check the connection status at the bottom of the JMRI window (should show "Connected")

**Messages appear but events don't match**:
- Verify the event IDs in your code match what you're looking for in JMRI
- Check that you didn't modify the EVENT_ID constants

**"Connection lost" after working initially**:
- ESP32 may have rebooted or lost WiFi connection
- Check the serial monitor for errors or WiFi reconnection attempts
- ESP32's IP address may have changed if DHCP lease renewed

## Troubleshooting

**Library not found during build:**
- Verify the `lib_deps` line in `platformio.ini` has no typos
- Run **PlatformIO: Clean** from the command palette (`Ctrl+Shift+P`)
- Rebuild the project

**ESP32 not detected:**
- Check USB cable (must be data cable, not charge-only)
- Install CP2102 or CH340 USB driver for your operating system
- Try a different USB port

**WiFi connection fails:**
- Verify SSID and password in code
- Check that ESP32 is within range of access point
- ESP32 only supports 2.4GHz WiFi (not 5GHz)

**JMRI not seeing events:**
- Verify JMRI is configured for TCP GridConnect connection
- Check that ESP32 and JMRI are on the same network
- Verify the TCP server address and port in ESP32 code

## What's Next

You've now built your first OpenLCB node with WiFi transport! 

**In the next chapter**, we'll explore:
- Adding physical GPIO inputs (buttons) and outputs (LEDs) to your breadboard
- Modifying the code to respond to button presses and control LEDs
- Understanding GPIO configuration and debouncing

**Beyond that**, future chapters will cover:
- Adding CAN hardware for traditional LCC bus communication
- Advanced event handling and CDI (Configuration Description Information)
- Memory configuration and parameter persistence
- SNIP (Simple Node Information Protocol) for node identification
- Train control and specialized protocols

For now, experiment with your working node: modify event IDs, add more inputs/outputs, or integrate it with other OpenLCB devices via JMRI.
