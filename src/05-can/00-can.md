# Switching to CAN Transport: From WiFi to Hardware

This chapter transitions your OpenLCB node from WiFi/TCP transport (Chapter 3) to CAN bus transport—the standard physical layer for most OpenLCB networks. You'll keep the same node code and configuration, but swap out the network interface for a hardware CAN connection via an external transceiver and a CAN bus interface module (such as RR-CirKits LCC Buffer-USB, or any compatible USB-to-CAN adapter).

## Overview

In Chapter 3, we built a working OpenLCB node that communicated over WiFi via a TCP Hub. While powerful for learning and for embedded systems with network connectivity, WiFi-based OpenLCB is not the standard deployment model. Real OpenLCB networks rely on **CAN (Controller Area Network)**, a robust, deterministic protocol designed for automotive and industrial control systems.

The good news: **the OpenLCB node code barely changes**. What does change is how the hardware communicates with the rest of your LCC network.

### What You'll Learn

- How CAN transceivers work and why they need external power
- The `Esp32HardwareTwai` class and why it's named that (technical context for readers using other tools)
- Wiring an ESP32 to a CAN transceiver on a breadboard
- Configuring JMRI to connect via a CAN bus interface module instead of TCP
- Diagnosing CAN bus issues using TWAI status messages
- Verifying your node works the same way on CAN as it did on WiFi

### Chapter Structure

This chapter assumes you've completed Chapters 3 and 4 (WiFi node and configuration). You'll need:

- **Hardware**: 
  - CAN transceiver (SN65HVD230 or MCP2551)
  - CAN bus interface module (e.g., RR-CirKits LCC Buffer-USB, or any USB-to-CAN adapter compatible with JMRI)
  - CAN bus power supply (e.g. RR-CirKits LCC Power-Point or equivalent)
  - 120Ω terminating resistors
  - breadboard
  - jumpers
- **Time**: ~1–2 hours depending on your familiarity with breadboard wiring
- **Existing knowledge**: Node code from Chapter 3, JMRI basics from Chapter 3, configuration fundamentals from Chapter 4

The sections guide you through:

1. **Understanding CAN Transceivers** — Open-collector design, power requirements, transceiver options
2. **Hardware Wiring** — Fritzing diagram and step-by-step breadboard setup
3. **Code Changes** — Minimal modifications to switch from WiFi to CAN
4. **The Esp32HardwareTwai Class** — Why it's named that, and what's coming in future Arduino cores
5. **JMRI Reconfiguration** — Disabling TCP, enabling RR-CirKits connection
6. **TWAI Bus Diagnostics** — Reading and interpreting status messages for troubleshooting
7. **Verification** — Confirming your node works the same way on CAN as it did on WiFi

> **Key Insight**: Your OpenLCB node is **transport-agnostic**. The application logic (producing events every 2 seconds) is completely separate from the physical layer (WiFi vs. CAN). This is the power of good architecture—change the transport, keep the application.
