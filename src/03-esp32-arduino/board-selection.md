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
