# OpenMRN-Lite Architecture & Capabilities

You've learned the concepts behind OpenLCB. Now, before we write code, let's understand the software library we'll be using: OpenMRN-Lite. This chapter clarifies what it is, why it's the right choice for ESP32, and what it can (and can't) do.

## What is OpenMRN-Lite?

**OpenMRN-Lite is the Arduino version of OpenMRN.**

This might sound like it's a cut-down or simplified version, but that would be misleading. Instead, think of it as a version optimized for single-threaded, resource-constrained environments like Arduino and ESP32.

There are two ways to run OpenMRN code:

| Version | Threading Model | Best For | Platform |
|---------|-----------------|----------|----------|
| **Full OpenMRN** | Multi-threaded (FreeRTOS) | Complex systems, command stations, bridges | Linux, macOS, Windows, native ESP-IDF |
| **OpenMRN-Lite** | Single-threaded executor | Learning, sensors, simple controllers | Arduino IDE, PlatformIO (Arduino framework) on ESP32, STM32 |

The key insight: **Arduino cannot run the multi-threaded version of OpenMRN.** The Arduino runtime environment doesn't provide the POSIX threading APIs that full OpenMRN requires. Therefore, OpenMRN-Lite is not a "lite" compromise—it's the only OpenMRN version available for Arduino.

## Why OpenMRN-Lite is the Right Choice for Learning OpenLCB

When building an OpenLCB node on ESP32 using Arduino, OpenMRN-Lite is the right tool for this learning path because of its simplicity and low barrier to entry.

**The Alternative**: You *could* use the full OpenMRN library with native ESP-IDF (Espressif's real-time operating system for ESP32). This gives you access to advanced features and the full power of FreeRTOS threading. But it also requires:
- Understanding real-time operating systems, threading, and synchronization primitives
- Managing POSIX APIs and FreeRTOS queues before you even write a single OpenLCB message
- Debugging complex multi-threaded interactions
- A completely different development environment and build system

For learning OpenLCB concepts, that's like learning to drive by starting with a race car instead of a regular car.

**Why OpenMRN-Lite Instead**: 
- **Familiar environment** - Arduino's `setup()`/`loop()` model is straightforward and widely understood
- **Focus on OpenLCB** - You can understand nodes, events, and producers/consumers without threading complexity
- **Fast results** - You'll have working code sending real OpenLCB messages within hours
- **Proven examples** - The IOBoard example demonstrates CDI configuration, events, and hardware I/O patterns you can build on
- **Natural progression** - Once you master OpenLCB concepts, you can migrate to ESP-IDF + full OpenMRN if needed

This isn't a permanent limitation—it's a strategic choice that lets you learn faster and more effectively. Once you understand OpenLCB deeply, you'll be better equipped to understand why those advanced features (virtual nodes, traction protocol, multi-threading) exist and when you might need them.

## What OpenMRN-Lite DOES Support

OpenMRN-Lite has everything you need to build real, functional OpenLCB nodes:

**Core Features** ✅

- **CDI (Configuration Description Information)** - Define configuration options that JMRI can edit without recompilation
- **SNIP (Simple Node Information Protocol)** - Share node name and description with the network
- **Event Producers & Consumers** - The core OpenLCB pattern you learned in Chapter 1
- **Datagrams** - Reliable message delivery for configuration and data exchange
- **ACDI (Abbreviated CDI)** - Simpler configuration interface for basic nodes
- **CAN Transport (optional)** - Add a CAN transceiver to use CAN bus instead of WiFi
- **TCP Hub (optional)** - Connect to JMRI over WiFi using GridConnect protocol
- **Factory Reset Patterns** - Reset to known states without recompilation
- **Persistent Configuration** - Store settings in SPIFFS or SD card

The IOBoard example (the most complete OpenMRN-Lite example) demonstrates all of these features in action: CDI-based configuration, event handling, hardware I/O, and network integration. It's exactly the pattern you'll use when building your own nodes.

## What OpenMRN-Lite Does NOT Support

There are some advanced features that require full OpenMRN with FreeRTOS threading:

**What You Don't Get** ❌

- **Virtual Nodes** - Hosting multiple logical nodes on one microcontroller
- **Traction Protocol** - Command station features (throttle control, trains)
- **Multi-Transport Bridging** - Routing messages between CAN and TCP automatically
- **Multi-Threaded I/O** - Background threads for independent subsystems
- **Hub Services** - Virtual topology management and advanced networking
- **Extensive Memory Configurations** - Very large configuration systems

These features aren't missing from OpenMRN-Lite because the developers cut corners. They're missing because they require the threading and memory management capabilities that FreeRTOS provides, which the Arduino framework doesn't offer.

**This is not a limitation for learning OpenLCB.** In fact, OpenMRN-Lite's simplicity makes it easier to understand how OpenLCB works without being buried in threading complexity.

## When to Use OpenMRN-Lite vs Full OpenMRN

Use this decision matrix to understand which tool is right for your project:

| Your Need | Use | Why |
|-----------|-----|-----|
| Learning LCC on ESP32 | OpenMRN-Lite | Single-threaded, Arduino-native, proven examples |
| Building a sensor node (button, LED, turnout) | OpenMRN-Lite | Minimal code, small footprint, stable |
| Fixed-function controller (no changes after deployment) | OpenMRN-Lite | No runtime configuration needed; extremely reliable |
| **Hosting multiple nodes on one ESP32** | ❌ Not possible with Arduino | Requires FreeRTOS; switch to native ESP-IDF + full OpenMRN |
| **Building a command station** | ❌ Not possible with Arduino | Requires traction protocol + FreeRTOS; use Linux or dedicated hardware |
| **Bridging CAN and TCP automatically** | ❌ Not possible with Arduino | Requires multi-threading; use Linux |

The key takeaway: **If you're using Arduino/PlatformIO on ESP32, OpenMRN-Lite is the only OpenMRN version available.** There is no "upgrade path" to full OpenMRN while staying in the Arduino ecosystem. If you need full OpenMRN features, you switch toolchains entirely (to native ESP-IDF + FreeRTOS), which requires rewriting in a different style.

## Configuration & Learning Implications

One important capability worth highlighting: **OpenMRN-Lite fully supports CDI**, the configuration system. This means you can:

1. **Define configuration options** in your code (node name, GPIO pins, event IDs, etc.)
2. **Connect to JMRI** and see those options in a graphical interface
3. **Change configuration** without recompiling or uploading new firmware
4. **Persist changes** to the ESP32's filesystem (SPIFFS)

This is powerful for learning because:
- You can experiment with event IDs without recompiling
- You understand how real OpenLCB nodes work (configuration happens at runtime)
- JMRI becomes a tool for monitoring *and* controlling your node

In this book's v0.1, we'll start with hardcoded configuration for simplicity. In later chapters (Chapter 5), we'll enhance the example to use CDI configuration, showing you the pattern for building production nodes.

## Looking Ahead

In Chapter 3, you'll see how OpenMRN-Lite integrates into a real ESP32 project using Arduino and PlatformIO. The library handles all the protocol details—your job is just to define events, read inputs, and write outputs.

And if you ever do need features that OpenMRN-Lite doesn't support, that's okay. You'll understand the OpenLCB concepts deeply enough to appreciate why those features exist and how they work in more complex systems. The goal of this book is to build that foundation.

