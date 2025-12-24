# Transports: WiFi/TCP and CAN

OpenLCB is designed to work over different physical transports. We use two main options across this book: **WiFi/TCP** (for rapid learning) and **CAN** (for real-world deployments).

## Phase 1: WiFi/TCP Transport (Quick Start)

We begin with **WiFi and TCP** to get your first OpenLCB node running in minutes:

**For Rapid Learning**:
- **No special hardware required** - ESP32 boards have WiFi built-in; no CAN transceivers needed
- **Easy monitoring** - Standard network tools and JMRI can capture all traffic
- **Faster iteration** - Wireless upload and debugging without physical bus connections
- **Focus on protocol** - Understand events, producers, and consumers without hardware distractions

**Key Insight**: The OpenLCB message formats, node startup, and event handling work **identically** over WiFi/TCP and CAN. Everything you learn in phase 1 transfers directly to CAN-based implementations in phase 2. TCP is a deliberate stepping stone, not a dead-end.

## Phase 2: CAN Bus Transport (Main Focus)

**CAN (Controller Area Network)** is the traditional transport for OpenLCB and is used in the vast majority of commercial LCC products. After mastering the protocol over TCP, you'll add CAN hardware and use it as your primary transport.

**Why CAN is the Real-World Standard**:
- **Excellent noise immunity** - designed for harsh environments (layout with motors, lights, switching supplies)
- **Built-in collision handling** - arbitration is hardware-enforced, not software-managed
- **Two-wire bus** - simple termination (120Ω at both ends)
- **Industry-proven** - decades of use in automotive and industrial settings
- **Multi-device scalability** - hundreds of nodes can share the same two-wire bus
- **Compatibility** - interoperability with commercial LCC products and installations

**What You'll Add**:
- A CAN transceiver board (~$5)
- Proper bus termination (two 120Ω resistors)
- A two-wire connection to other nodes

**When Phase 2 Starts**: Chapters on inputs and outputs focus on CAN. You'll explore multi-node systems, real-world layouts, and the patterns used in most model railroad installations.

## Phase 3: Multi-Transport Integration (Advanced)

Once you've mastered CAN-based I/O, you may eventually need to:
- Bridge TCP and CAN on the same device
- Implement virtual nodes
- Use the traction protocol for command stations

These advanced features require full OpenMRN with FreeRTOS threading, covered in later chapters.

## The Progression

This is one of OpenLCB's strengths: the **protocol is transport-agnostic**. You can:

1. **Start with WiFi/TCP** on your workbench (phase 1—no special hardware)
2. **Learn OpenLCB fundamentals** (events, producers, consumers, CDI)
3. **Add CAN hardware** and switch to phase 2 (same code patterns; now using the real transport)
4. **Integrate both transports** if needed (phase 3, only when necessary)

The key insight: learning with TCP doesn't waste your time. It's a deliberate stepping stone that lets you focus on protocol before worrying about hardware. When you move to CAN, the only change is the physical connection and a few hardware initialization lines—your OpenLCB knowledge applies perfectly.
