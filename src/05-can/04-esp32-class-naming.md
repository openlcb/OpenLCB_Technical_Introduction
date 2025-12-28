# The Esp32HardwareTwai Class: Arduino Cores and the Name Change

When you look at different OpenMRNLite examples online, you might see both `Esp32HardwareTwai` and `Esp32Can` used for CAN on the ESP32. What's the difference, and which one should you use? The answer depends on your **Arduino core version and development environment**.

**Note**: This chapter uses **PlatformIO with Arduino Core v2.x**. As mentioned in Chapter 2, at the time of writing (December 2025), newer Arduino core versions are not yet available in PlatformIO. The information below about Arduino Core v3 is primarily for readers using the **Arduino IDE** or **Arduino Maker Workshop** (which use the Arduino CLI and can access newer core versions).

## Why the Class Name Changed

### The Short Answer

Espressif (the company that makes the ESP32) changed the underlying **driver naming** between ESP-IDF v4.x and v5.x:

- **ESP-IDF v4.x** (used by Arduino Core v2): Driver was called **TWAI** (Two-Wire Automotive Interface)
- **ESP-IDF v5.x** (used by Arduino Core v3): Driver is now called **CAN**

OpenMRNLite updated its class names to match the Arduino core version:

- **OpenMRNLite 2.0.x - 2.1.x** (Arduino Core v2) → `Esp32HardwareTwai`
- **OpenMRNLite 2.2.0+** (released August 2025, Arduino Core v3) → `Esp32Can`

**Important**: OpenMRNLite v2.2.0 marked a significant shift—the v2.x line switched from Arduino Core v2 to Arduino Core v3. If you're using an older version of OpenMRNLite (2.1.x or earlier), use `Esp32HardwareTwai`. If you're on v2.2.0 or later, use `Esp32Can`.

It's the same hardware, same functionality, but the class name and Arduino core changed.

### Why Did Espressif Rename It?

Espressif originally named the ESP32's CAN-like peripheral **TWAI** (Two-Wire Automotive Interface) because the hardware does not implement the *full* CAN 2.0 specification. The ESP32's controller is a simplified CAN implementation with reduced filtering and feature support—it's "CAN-compatible" but not fully CAN-compliant. Espressif chose the name TWAI to avoid implying full CAN compliance and to give themselves flexibility for future chips with different CAN-like peripherals.

As developers overwhelmingly referred to the peripheral as "CAN" and because it works reliably with standard CAN transceivers and protocols, OpenMRNLite (and other libraries) later adopted the more familiar `Esp32Can` naming. The hardware itself never changed—only the naming conventions evolved to match developer expectations.

## What You're Using Today

This chapter uses **Arduino Core v2.x** (as defined in your `platformio.ini`):

```ini
platform = espressif32@6.12.0  # This version uses Arduino Core v2
lib_deps = openmrn/OpenMRNLite@2.0.0
```

This means you use:

```cpp
Esp32HardwareTwai can_driver(CAN_RX_PIN, CAN_TX_PIN);
can_driver.hw_init();
openmrn.add_can_port_select("/dev/twai/twai0");
```

## If You're Using Arduino IDE or Arduino Maker Workshop

If you're using Arduino Core v3 through the Arduino IDE or Arduino Maker Workshop (instead of PlatformIO), the code changes are straightforward:

```cpp
// Arduino Core v3 (Arduino IDE / Maker Workshop only)
Esp32Can can_driver(CAN_RX_PIN, CAN_TX_PIN);  // Note: "Esp32Can", not "Esp32HardwareTwai"
can_driver.hw_init();
openmrn.add_can_port(&can_driver);
```

You'd also need to upgrade OpenMRNLite to v2.2.0 or later. The hardware and functionality remain identical—only the class name and library version change.

> **Key Takeaway**: For **PlatformIO users** (this guide), use `Esp32HardwareTwai` with Arduino Core v2.x. We won't be upgrading in future chapters—PlatformIO v6.12.0 is stable and well-supported.
>
> For **Arduino IDE users** considering an upgrade: The changes are straightforward (class name, device path, library version), but you should only upgrade if you have a specific reason (newer board support, bug fixes, etc.). The current Arduino Core v2.x works reliably for OpenLCB CAN nodes.

**Next**: Configure JMRI to connect via CAN instead of WiFi.
