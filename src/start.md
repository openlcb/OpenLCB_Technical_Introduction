# Getting Started

In the previous chapter, you learned about OpenLCB concepts: nodes, transport layers, startup sequences, and the producer/consumer event model. Now it's time to build something real.

This chapter will guide you through the choices we've made for this book and prepare you for hands-on work in the next chapter.

## What We're Building

We're going to build a simple but complete OpenLCB node that demonstrates the core concepts:

**Hardware**: An ESP32 microcontroller with:
- A push button (input) - produces an OpenLCB event when pressed
- An LED (output) - consumes OpenLCB events to turn on/off
- WiFi connectivity - connects to your home network

**Software**: 
- OpenMRNLite library - provides the complete OpenLCB protocol stack
- Arduino framework - familiar development environment
- PlatformIO - modern build system and IDE integration

**Network Integration**:
- JMRI monitoring tool - see OpenLCB messages in real-time
- TCP/GridConnect protocol - network-based communication
- Multi-node capable - your node can interact with other OpenLCB devices

This example is intentionally simple to focus on learning. Real layout nodes can have dozens of inputs/outputs, configuration systems, and specialized protocols—we'll cover those in future chapters.



## Library Choice: OpenMRN-Lite

Before choosing a microcontroller, we need to choose an OpenLCB library. There are two main options:

**Full OpenMRN** uses FreeRTOS and POSIX threading, giving you access to advanced features like virtual nodes and traction protocol. But it requires Linux, macOS, Windows, or native ESP-IDF—a full real-time operating system environment. The learning curve is steep: you need to understand threading, mutexes, message queues, and how RTOS schedulers work *before* you can focus on OpenLCB itself.

**OpenMRN-Lite** is the Arduino version: single-threaded, no RTOS required, designed specifically for learning and hobby projects. It has everything you need for your first OpenLCB node (events, producers/consumers, CDI configuration, CAN/TCP support). Most importantly, it lets you focus on OpenLCB concepts without fighting with threading complexity.

**We're choosing OpenMRN-Lite** for this book because:

- **Lower learning curve** - Arduino's simple `setup()`/`loop()` model is familiar to most hobbyists and embedded developers
- **Faster progress** - You'll have working code and see results within hours, not days
- **Proven examples** - OpenMRN-Lite has excellent examples (IOBoard, WifiCanBridge) that demonstrate real patterns
- **Future flexibility** - Once you understand OpenLCB concepts, upgrading to full OpenMRN (with FreeRTOS) becomes a natural next step, not a barrier

This is not a limitation—it's a pedagogical choice. You can absolutely build production OpenLCB nodes with OpenMRN-Lite. Chapter 2.5 goes deeper into what OpenMRN-Lite supports and when you might want to switch toolchains.

## Platform Choice: ESP32

We've chosen the ESP32 microcontroller platform for this book because it's one of the best-supported platforms for OpenMRN-Lite:

**Why ESP32 for OpenMRN-Lite?**
- **Excellent Arduino support** - Arduino framework is mature and stable on ESP32
- **WiFi built-in** - Perfect match for our WiFi/TCP transport choice
- **CAN capable** - Built-in CAN controller for adding CAN hardware in future chapters
- **Affordable** - Development boards are $5-15 USD
- **Powerful** - Dual-core processor, plenty of memory for OpenMRNLite applications
- **GPIO-rich** - Enough pins for inputs, outputs, and expansion

**Other Platforms with OpenMRN-Lite Support**: OpenMRN-Lite works on other Arduino-compatible boards:
- STM32 family (Nucleo boards) via Arduino core
- Arduino variants (Mega, Due)
- Other ESP32 variants (ESP32-S3, ESP32-C3)

If you have a different board available, you can likely adapt the examples. Chapter 2.5 has more details on platform trade-offs. For now, ESP32 is the best starting point: affordable, well-documented, and fully supported by the OpenMRN-Lite community.

## Why WiFi/TCP First?

As mentioned in Chapter 1, OpenLCB works over multiple physical transports. We're starting with WiFi and TCP for several practical reasons:

**For Learning**:
- **No special hardware required** - ESP32 boards have WiFi built-in, no CAN transceivers needed
- **Easy monitoring** - standard network tools and JMRI can capture all traffic
- **Faster iteration** - wireless upload and debugging without physical bus connections
- **Lower barrier to entry** - get started with just a microcontroller and breadboard

**Same Protocols**: The OpenLCB message formats, node startup, and event handling work identically over WiFi/TCP and CAN. Everything you learn here transfers directly to CAN-based implementations.

**CAN Is Still Important**: Traditional LCC installations use CAN bus, and for good reason:
- Excellent noise immunity for layout environments
- Built-in collision handling and arbitration
- Two-wire bus with simple termination
- Industry-proven reliability

We'll cover adding CAN hardware in future chapters. For now, WiFi/TCP lets you focus on understanding OpenLCB without hardware complexity.

## Prerequisites & Assumptions

Before diving into the next chapter, make sure you have:

**Knowledge**:
- Basic embedded programming (C/C++)
- Microcontroller I/O concepts (GPIO, digital read/write)
- Familiarity with breadboards and simple circuits
- Producer/consumer model understanding (from using LCC products)

**Hardware** (needed for Chapter 3):
- ESP32 development board (ESP32 DevKit v1 or similar)
- USB cable for programming
- Breadboard and jumper wires
- Push button (momentary, normally open)
- LED (any color)
- 220Ω resistor (for LED current limiting)
- Computer with WiFi (for network connection)

**Software** (installation covered in Chapter 3):
- VS Code or similar editor
- PlatformIO extension
- JMRI (for monitoring OpenLCB traffic)

If you're missing any hardware, most items can be purchased as a kit from electronics suppliers (Adafruit, SparkFun, Amazon, AliExpress) for under $20 USD total.

## Development Environment Overview

We'll be using modern, cross-platform tools:

**PlatformIO**: A professional build system and IDE extension that handles:
- Automatic toolchain installation
- Library dependency management
- Serial monitoring and debugging
- Support for hundreds of boards and frameworks

**Arduino Framework**: Provides:
- Familiar `setup()` and `loop()` structure
- Standard GPIO functions (`pinMode`, `digitalWrite`, etc.)
- WiFi libraries for ESP32
- Large ecosystem of compatible libraries

**OpenMRNLite Library**: The core OpenLCB implementation:
- Complete protocol stack (startup, events, datagrams, streams)
- SimpleStack API for easy setup
- Event producers and consumers
- TCP and CAN transport support
- Memory-efficient design for microcontrollers

Chapter 3 will walk through installation and configuration step-by-step. No prior PlatformIO experience required.

## Understanding OpenMRN-Lite

Before we dive into code, you might be wondering: **What exactly is OpenMRN-Lite? Is it a cut-down version? Will I outgrow it?**

Chapter 2.5 ("OpenMRN-Lite Architecture & Capabilities") answers these questions in detail. It explains:
- Why OpenMRN-Lite is the *only* OpenMRN version that runs on Arduino (not a limitation, just a fact)
- What features it supports (CDI, events, datagrams, CAN, and more)
- When you might want to switch toolchains entirely (and when you won't)

For now, know this: **OpenMRN-Lite is the right tool for learning OpenLCB on ESP32.** It has everything you need to build real nodes, and understanding how it works sets you up perfectly for more advanced projects later.

## Monitoring with JMRI (TCP)

JMRI (Java Model Railroad Interface) is an essential tool for working with OpenLCB:

**Why JMRI?**
- **Message decoder** - Translates raw hex into readable OpenLCB messages
- **Network monitor** - See all CID, RID, AMD, event messages in real-time
- **Testing tool** - Send events to your node and verify responses
- **Layout integration** - Connect your node to a larger LCC network

**TCP/GridConnect Protocol**: JMRI connects to your ESP32 node over WiFi using:
- TCP socket connection (default port 12021)
- GridConnect ASCII format: `:X<header>N<data>;`
- Human-readable message traces
- Standard format used by many OpenLCB tools

**What You'll See**: When your node starts up, JMRI shows:
1. Four CID frames (checking alias availability)
2. RID frame (reserving alias)
3. AMD frame (mapping Node ID to alias)
4. Initialized message (node is online)
5. Producer/Consumer Identified messages (node capabilities)
6. Event reports (button presses, LED changes)

Chapter 3 includes detailed JMRI setup and verification steps. You'll see your first OpenLCB messages within minutes of uploading code.

For a conceptual overview of network architecture (hubs, nodes, and the diagram), see the Node overview: [Node](./node_architecture.md#network-architecture).

## What's Next

Now that you understand the platform choices and tools, Chapter 3 will guide you through:

1. **Installing PlatformIO** - Step-by-step setup in VS Code
2. **Installing OpenMRNLite** - Adding the library to your project
3. **Building async_blink_esp32** - Complete walkthrough of the example code
4. **Deploying to ESP32** - Upload and run your first OpenLCB node
5. **Verification with JMRI** - See your node join the network and exchange events over WiFi
6. **Troubleshooting** - Common issues and solutions

By the end of Chapter 3, you'll have a working OpenLCB node that you built yourself. You'll understand the code, see the messages, and be ready to create your own custom nodes.

Chapter 4 will then cover adding physical buttons and LEDs to interact with the real world.

Let's get started!

