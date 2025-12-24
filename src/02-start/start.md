# Getting Started

In the previous chapter, you learned about OpenLCB concepts: nodes, transport layers, startup sequences, and the producer/consumer event model. Now it's time to plan your first OpenLCB node.

This chapter walks you through the key decisions we'll make for building your node and explains the **why** behind each choice so you understand the trade-offs and can adapt to your own needs. (Actual implementation begins in the next chapter.)

## What We're Building

The goal of this book is to help you create an **OpenLCB node**—a device that can produce and consume events on an LCC network. Your node will:

- **Sense inputs** (buttons, switches, sensors) and produce OpenLCB events
- **Control outputs** (LEDs, relays, motors) by consuming OpenLCB events
- **Connect to a network** so other nodes can respond to your events
- **Be monitored and configured** via standard tools like JMRI

We'll start with a simple example and scale to more complex scenarios in later chapters. The key concept is **the pattern**: once you understand how to produce and consume a single event, scaling to many inputs/outputs and adding complex logic becomes straightforward.

### Progression: TCP, Then CAN, Then Full OpenMRN

We use three key technologies across this book, introduced progressively:

**Phase 1: TCP/WiFi (Quick Start)**
- Use **WiFi/TCP transport** with **Arduino framework** and **OpenMRN-Lite** library to get your first node running in minutes
- Focus on OpenLCB fundamentals (events, producers, consumers, CDI) without hardware barriers
- No CAN transceiver required—just an ESP32 and JMRI for monitoring
- Once you understand the protocol, adding CAN is straightforward

**Phase 2: CAN Transport (Main Focus)**
- Add a **CAN transceiver** (~$5) and switch from **WiFi/TCP to CAN** transport
- Same **Arduino framework** and **OpenMRN-Lite** library as phase 1
- Explore inputs and outputs with the real transport layer used in most LCC installations
- Build multi-node systems that communicate over CAN
- Stay here if your needs fit this model (most hobby layouts do)

**Phase 3: Full OpenMRN + FreeRTOS (Advanced)**
- Only if you need multi-threading, virtual nodes, or traction protocol
- Migrate from Arduino to **ESP-IDF** (Espressif's full RTOS environment)
- Use **full OpenMRN** library with FreeRTOS threading
- Handle complex, multi-threaded scenarios (bridges, command stations, etc.)

Each phase builds on the previous one. The OpenLCB knowledge transfers directly—what changes is the platform and threading model.

### Key Decisions

Building an OpenLCB node requires several key decisions, explained in the later sections of this chapter:

1. **Which programming framework?** — Arduino for phases 1–2, FreeRTOS only if phase 3 is needed
2. **Which OpenLCB library?** — OpenMRN-Lite (Arduino) for phases 1–2, full OpenMRN only for phase 3
3. **Which platform?** — ESP32 (recommended), STM32, or other microcontroller
4. **Which IDE and build tools?** — PlatformIO (recommended), Arduino IDE, or native ESP-IDF
5. **Which transport?** — TCP/WiFi for phase 1 (quick learning), CAN for phase 2 (main focus), both for phase 3
6. **How do I monitor and verify?** — JMRI monitoring tool, TCP hub architecture, CAN bus analysis

By the end of this chapter, you'll understand the **reasons** behind each choice and why this progression makes sense for learning and building real LCC systems.

## Detailed Topic Coverage

- [Arduino for Early Chapters (Migration Path)](./arduino-and-migration.md) — Why Arduino; how to migrate to FreeRTOS later
- [OpenMRN-Lite Architecture & Capabilities](./openmrn-architecture.md) — Why OpenMRN-Lite for Arduino; capabilities and limitations
- [Platform: ESP32 & SPIFFS](./platform-esp32.md) — Why ESP32; the importance of persistent storage
- [Development Environments & Tooling](./development-environments.md) — IDE comparison (Arduino IDE, Maker Workshop, PlatformIO); personal recommendation with GitHub Copilot
- [Transports: WiFi & CAN](./platform-and-transports.md) — Why WiFi first; when to add CAN
- [Monitoring & Verification](./monitoring-and-tools.md) — JMRI, TCP hub, GridConnect protocol

## Prerequisites

Before reading further, make sure you have:

**Knowledge**:
- Basic embedded programming (C/C++)
- Microcontroller I/O concepts (GPIO, digital read/write)
- Familiarity with breadboards and simple circuits
- Producer/consumer model understanding (from using LCC products)

**Hardware** (needed for Chapter 4):
- ESP32 development board (ESP32 DevKit v1 or similar)
- USB cable for programming
- Computer with WiFi
- Optional: breadboard, jumper wires, push button, LED, resistor (for hardware integration in Chapter 5)

**Software** (installation covered in Chapter 4):
- VS Code or similar editor
- PlatformIO extension (or Arduino IDE / Maker Workshop)
- JMRI (for monitoring OpenLCB traffic)

If you're missing any hardware, most items can be purchased as a kit from electronics suppliers (Adafruit, SparkFun, Amazon, AliExpress) for under $20 USD total.

## What's Next

You now have the big picture: a generic goal (inputs → events → outputs) plus the concrete stack for early chapters. Later sections in this chapter dive into each decision.

Start with **[Arduino for Early Chapters (Migration Path)](./arduino-and-migration.md)**. Each section builds on the previous one, culminating in a clear understanding of our toolchain and ready-to-implement design.

By the end of this chapter, you'll be prepared to move forward with the example implementation.


