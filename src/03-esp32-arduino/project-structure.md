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
platform = espressif32@6.12.0
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
```

## Adding OpenMRNLite to Your Project

OpenMRNLite is the lightweight version of OpenMRN designed for Arduino-compatible platforms. It provides all the core LCC/OpenLCB functionality without the full complexity of the OpenMRN framework.

### Installation via platformio.ini

Open the `platformio.ini` file in your `async_blink_esp32` project and add OpenMRNLite to the library dependencies:

```ini
[env:esp32doit-devkit-v1]
platform = espressif32@6.12.0
board = esp32doit-devkit-v1
framework = arduino
lib_deps = openmrn/OpenMRNLite@2.0.0
monitor_speed = 115200
```

Save the file. That's it! PlatformIO will automatically download OpenMRNLite from the registry when you build the project.

**About OpenMRNLite version 2.0.0**: We're using v2.0.0 rather than newer versions because:
- Version 2.0.0 is fully compatible with PlatformIO's current ESP32 platform
- Later versions (2.2.x+) require newer ESP-IDF features not yet available in PlatformIO
- All core OpenLCB functionality is present in v2.0.0

**About monitor_speed**: This setting ensures the serial monitor uses 115200 baud, matching the `Serial.begin(115200)` call in our code. Without this, you'll see garbled output instead of readable text.

> **About platform versions and Arduino-ESP32**
>
> Arduino-ESP32 has two major versions: 2.x and 3.x. The Arduino IDE now supports 3.x, but PlatformIO still uses 2.x. The `platform = espressif32@6.12.0` pins us to PlatformIO's current 2.x series.
>
> Why not upgrade to Arduino-ESP32 3.x?
> - **Arduino-ESP32 3.x is still evolving** with breaking changes to the build system, partition handling, WiFi stack, and more.
> - **PlatformIO prioritizes stability** over bleeding-edge features. They thoroughly test each framework version across hundreds of boards, toolchains, and debuggers before releasing.
> - **Smaller maintenance burden**: By using a tested, stable version, we reduce variables when troubleshooting issues.
>
> OpenMRNLite v2.0.0 is designed for the Arduino-ESP32 2.x that PlatformIO provides via espressif32@6.12.0. You're trading bleeding-edge features for rock-solid reliability, which is perfect for learning OpenLCB fundamentals.
>
> **Note**: The code in this chapter should also work with Arduino-ESP32 3.x using either the Arduino IDE or Arduino Maker Workshop, but we haven't tested those configurations.

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
