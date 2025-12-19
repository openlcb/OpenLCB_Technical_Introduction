# ESP32 with Arduino & PlatformIO

This chapter covers everything needed to build and deploy an OpenLCB node on an ESP32 microcontroller using Arduino and PlatformIO. We'll create a working example that you can modify for your own applications.

## Overview

The ESP32 is a powerful, affordable microcontroller with built-in WiFi connectivity, making it ideal for learning OpenLCB concepts. In this chapter, we'll use WiFi and TCP transport to communicate over a network, which is simpler than CAN hardware and provides immediate visibility into node behavior.

## PlatformIO Installation & Setup

(Content to be added: Installation guide for VS Code with PlatformIO extension, platform and toolchain setup)

## ESP32 Board Selection

(Content to be added: Recommended ESP32 boards, pin availability, memory/performance considerations)

## OpenMRNLite Library Installation

(Content to be added: Adding OpenMRNLite library via Arduino Library Manager or from source, configuration verification)

## Hardware Setup: Breadboard Circuit

(Content to be added: GPIO pin assignment, button wiring diagram, LED wiring with resistor, breadboard layout, bill of materials, connection verification steps)

## Building async_blink_esp32

(Content to be added: Project structure, platformio.ini template, WiFi configuration, building with PlatformIO)

## Deploying to ESP32

(Content to be added: Serial port selection, upload process, monitoring serial output, debugging output interpretation)

## Verification & Troubleshooting

(Content to be added: Expected startup messages, verifying WiFi connection, JMRI event verification, common issues and solutions)

## What's Next

You've now built your first OpenLCB node with WiFi transport! Future chapters will explore:

- Adding CAN hardware for traditional LCC bus communication
- Advanced event handling and CDI (Configuration Description Information)
- Memory configuration and parameter persistence
- SNIP (Simple Node Information Protocol) for node identification
- Train control and specialized protocols

For now, experiment with your working node: modify event IDs, add more inputs/outputs, or integrate it with other OpenLCB devices via JMRI.
