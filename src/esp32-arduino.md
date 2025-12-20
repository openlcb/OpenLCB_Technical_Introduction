# ESP32 with Arduino & PlatformIO

This chapter covers everything needed to build and deploy an OpenLCB node on an ESP32 microcontroller using Arduino and PlatformIO. We'll create a working example that you can modify for your own applications.

## Overview

The ESP32 is a powerful, affordable microcontroller with built-in WiFi connectivity, making it ideal for learning OpenLCB concepts. In this chapter, we'll use WiFi and TCP transport to communicate over a network, which is simpler than CAN hardware and provides immediate visibility into node behavior.

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
```

## Adding OpenMRNLite to Your Project

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
```

Save the file. That's it! PlatformIO will automatically download OpenMRNLite from the registry when you build the project.

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

(Content to be added: Complete code walkthrough, event producers/consumers, WiFi configuration)

## Building and Uploading to ESP32

(Content to be added: Build process, upload steps, serial monitoring)

## Testing with JMRI (Software Only)

(Content to be added: JMRI setup, event verification, testing event production/consumption without hardware)

## Adding Physical Hardware (Optional)

Once you've verified your software works with JMRI, you can optionally add physical button and LED components.

### Bill of Materials

- Solderless breadboard
- Tactile pushbutton switch
- LED (any color, 5mm recommended)
- 220Ω resistor (for LED current limiting)
- Jumper wires (male-to-male)

### GPIO Pin Assignment

For the ESP32 DevKit V1, we'll use:
- **GPIO 12**: Button input (with internal pull-up)
- **GPIO 13**: LED output

(Content to be added: Detailed wiring diagram, breadboard layout, connection verification)

## Troubleshooting

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

You've now built your first OpenLCB node with WiFi transport! Future chapters will explore:

- Adding CAN hardware for traditional LCC bus communication
- Advanced event handling and CDI (Configuration Description Information)
- Memory configuration and parameter persistence
- SNIP (Simple Node Information Protocol) for node identification
- Train control and specialized protocols

For now, experiment with your working node: modify event IDs, add more inputs/outputs, or integrate it with other OpenLCB devices via JMRI.
