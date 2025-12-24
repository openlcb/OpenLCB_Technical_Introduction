# Platform: ESP32 & SPIFFS

Now that we've committed to Arduino and OpenMRN-Lite, the next decision is: **which microcontroller platform?** We've chosen **ESP32** for this book. Here's why.

## Why ESP32?

The ESP32 platform is one of the best-supported microcontrollers for OpenMRN-Lite:

**Essential Features**:
- **Excellent Arduino support** - Arduino framework is mature and stable on ESP32
- **WiFi built-in** - Perfect match for our WiFi/TCP transport choice (no separate shield needed)
- **CAN capable** - Built-in CAN controller for adding CAN hardware in future chapters
- **Persistent storage (SPIFFS)** - Critical for OpenMRN configuration management (more on this below)

**Practical Advantages**:
- **Affordable** - Development boards are $5–15 USD
- **Powerful** - Dual-core processor, plenty of memory for OpenMRNLite applications
- **GPIO-rich** - Enough pins for inputs, outputs, and future expansion

## SPIFFS: Why It Matters

OpenMRN-Lite requires a **persistent filesystem** to store configuration data. When your node starts, it reads a configuration file that contains:
- Node identity (SNIP data: device name and description)
- Configuration schema definition (CDI)
- User settings (to be added in later chapters)

**SPIFFS** (SPI Flash File System) is ESP32's built-in filesystem. It allows OpenMRN to:
1. Store a configuration file on the device's flash memory
2. Read and update configuration without needing a separate EEPROM chip
3. Persist settings across power cycles

This means:
- No external hardware required (everything is on-chip)
- You can change node name and description via JMRI without recompiling firmware
- Future configuration parameters (WiFi settings, event mappings, etc.) can be edited live

Other platforms (Arduino Mega, STM32 Nucleo boards) either lack SPIFFS or require external EEPROM components, adding complexity. ESP32 has it built-in.

## Other Platforms with OpenMRN-Lite Support

OpenMRN-Lite works on other Arduino-compatible boards:

- **STM32 family** (Nucleo boards) via Arduino core - good option but requires external EEPROM for storage
- **Arduino Mega** - more memory than Uno, but no WiFi; requires WiFi shield
- **Other ESP32 variants** (ESP32-S3, ESP32-C3) - similar capabilities, also good choices

If you have a different board available, you can likely adapt the examples. Chapter 3 has more details on platform trade-offs. For now, **ESP32 DevKit v1** is the best starting point: affordable, well-documented, and fully supported by the OpenMRN-Lite community.

## Next: Choosing Your Development Environment

With platform and toolchain locked in (Arduino + OpenMRN-Lite + ESP32), the next step is choosing an IDE and build system. That's covered in the next section.
