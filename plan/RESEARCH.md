# Research Findings & References

## Key Information About async_blink Example

### Location & Build System
- **Main source**: `openmrn/applications/async_blink/main.cxx` (~265 lines)
- **Build targets**: `openmrn/applications/async_blink/targets/` (26 platforms)
- **Build system**: Recursive Makefile (legacy, not CMake)
- **Supported platforms**:
  - Desktop: Linux (x86, armv7a, llvm), macOS
  - STM32: Nucleo variants, Discovery boards
  - TI: LaunchPad (multiple variants)
  - NXP: LPC variants
  - Microchip: PIC32MX
  - Emulation: JavaScript/Emscripten
  - ESP8266: Non-OS variant (no WiFi-specific config)

### What It Does
- Produces/consumes 2 events, toggling between them every 1 second
- Event IDs: `0x0502010202000000` and `0x0502010202000001`
- Demonstrates node startup sequence (CID/RID/AMD/Init Complete)
- Runs on desktop WITHOUT CAN hardware (uses TCP on Linux/macOS)
- Outputs GridConnect-format messages

### Build Example
```bash
cd openmrn/applications/async_blink/targets/linux.x86
make
./async_blink
```

### Notes for async_blink_esp32 Adaptation
- Code pattern is reusable for ESP32; main differences:
  - Transport: TCP/IP (same on ESP32) vs. CAN frame handling
  - Hardware: GPIO for button/LED (NEW for ESP32 version)
  - Setup: WiFi connection required (NEW for ESP32 version)
  - Event IDs: Can reuse same ones or define new ones
- **NEW**: OpenMRNLite is now local! Check `OpenMRNLite/examples/ESP32/` for working patterns to adapt

---

## ESP32 & Arduino OpenMRNLite Library

### Library Location & Status
- **Main library (now local)**: `OpenMRNLite/` at workspace root
- **Examples**: `OpenMRNLite/examples/ESP32/` ✅ Perfect reference for async_blink_esp32
- **Arduino Library Manager**: Also available as "OpenMRNLite"
- **Status**: ✅ Actively maintained for ESP32
- **Note**: Having local source means we can examine examples directly instead of documentation

### Supported Platforms
1. **ESP32** ✅ Full support
   - arduino-esp32 core v1.0.1 or v1.0.2-rc2
   - Both WiFi and CAN modes
   - Multiple examples available
   
2. **STM32** ⚠️ Recognized but not actively documented
3. **ESP8266** ❌ NOT supported by OpenMRNLite Arduino

### Available Arduino Examples
- `IOBoard` - ESP32 with WiFi + CAN, CDI configuration
- `CanLoadTest` - CAN stress testing
- `WifiCanBridge` - WiFi to CAN bridge
- `CanSerialNode` - Feather M4 CAN board
- `ClockConsumer` - Event consumption example

### CAN Hardware Support (for future chapters)

**Recommended Transceivers**:
- SN65HVD23x (VP230) - 3.3V with on-board termination (preferred for ESP32)
- MCP2551 - 5V (requires external termination)

**GPIO Wiring Requirements**:
- VCC/VDD (3.3V or 5V based on transceiver)
- GND
- RX GPIO (configurable, needs 1k resistor if using 5V MCP2551)
- TX GPIO (NOT GPIO 34-39 as these are input-only)

**Bus Termination**: 120Ω resistor across CAN H/L at both ends

**Power Requirements**: 
- ⚠️ Do NOT connect CAN bus PWR_POS directly to ESP32 VIN
- Must use DC-DC buck converter
- Minimum 750mA supply

**NOTE**: Library uses raw transceiver wiring; no Arduino CAN shield-specific implementations documented.

---

## OpenMRN Stack Components (For Reference)

These are key files in the OpenMRN C++ implementation that async_blink_esp32 will leverage:

### Node Initialization & Core
- `openmrn/src/openlcb/Node.cxx/hxx` - Base node class
- `openmrn/src/openlcb/NodeInitializeFlow.cxx/hxx` - Startup sequence (CID/RID/AMD/Init Complete)
- `openmrn/src/openlcb/SimpleStack.cxx/hxx` - Complete protocol stack
- `openmrn/src/openlcb/AliasAllocator.cxx/hxx` - Alias negotiation
- `openmrn/src/openlcb/DefaultNode.cxx/hxx` - Default configuration

### Transport Layers
- `openmrn/src/openlcb/IfCan.cxx/hxx` - CAN interface
- `openmrn/src/openlcb/IfTcp.cxx/hxx` - TCP interface (used for WiFi)

### Event Handling
- `openmrn/src/openlcb/EventHandler.cxx/hxx`
- `openmrn/src/openlcb/EventService.cxx/hxx`
- `openmrn/src/openlcb/CallbackEventHandler.hxx`

### Features (For Future Chapters)
- `openmrn/src/openlcb/MemoryConfig.cxx/hxx` - Memory config protocol
- `openmrn/src/openlcb/SimpleNodeInfo.cxx/hxx` - SNIP
- `openmrn/src/openlcb/BroadcastTime.cxx/hxx` - Broadcast time sync
- `openmrn/src/openlcb/TractionTrain.cxx/hxx` - Train control

---

## Technical Standards Reference

Available in `markdown/standards/` folder. These are the authoritative references:

### Core Protocols (v0.1 Relevant)
- `S-9.7.0.3-UniqueIdentifiers` - Node IDs
- `S-9.7.0.4-EventIdentifiers` - Event IDs
- `S-9.7.1.1-CanPhysical` - CAN layer (reference, not needed for WiFi v1)
- `S-9.7.3-MessageNetwork` - Message structure
- `S-9.7.3.1-EventTransport` - Event messages
- `S-9.7.3.2-DatagramTransport` - Datagram protocol

### Configuration & Info (For Future Chapters)
- `S-9.7.4.1-ConfigurationDescriptionInformation` - CDI
- `S-9.7.4.3-SimpleNodeInformation` - SNIP
- `S-9.7.4.6-TrainControl` - Traction protocol

### Documentation Format
- PDF converted to Markdown (see PowerShell scripts in `markdown/`)
- Each standard has corresponding Technical Note (TN-9.7.x.x) with more detail
- Standards are stable; use as primary reference, TN for deep dives

---

## Documentation Gaps in Current openmrn.md

**What's Missing**:
1. ❌ How to build async_blink (mentioned but not detailed)
2. ❌ Platform-specific variations
3. ❌ Arduino/ESP32 setup completely absent
4. ❌ CAN transceiver hardware wiring
5. ❌ GridConnect protocol explanation
6. ❌ Event definition walkthrough
7. ❌ SNIP (Simple Node Info) setup
8. ❌ CDI (Configuration) creation
9. ❌ Windows-specific guidance (not needed for v1)
10. ❌ Troubleshooting section

**Plan for v0.1**: Create new `esp32-arduino.md` chapter addressing items 1, 3, 8, and provide updated JMRI/TCP guidance.

---

## Diagram Opportunities & Mermaid Examples

### High-Priority Diagrams for v0.1

#### 1. Node Startup Sequence (Sequence Diagram)
**Purpose**: Show timing and order of startup messages
```
Participant: ESP32 Node, CAN Bus, JMRI (Monitor)

ESP32 → Bus: CID frame 1 (node ID byte 1)
ESP32 → Bus: CID frame 2 (node ID byte 2)
ESP32 → Bus: CID frame 3 (node ID byte 3)
ESP32 → Bus: CID frame 4 (node ID byte 4)
[Pause for collision detection]
ESP32 → Bus: RID (Reserve ID)
ESP32 → Bus: AMD (Alias Map Definition)
ESP32 → Bus: Initialize Complete
```

#### 2. CAN Message Structure (Block Diagram)
**Purpose**: Explain 29-bit identifier breakdown
```
29-bit CAN ID:
[Priority (3 bits)] [Frame Type (4 bits)] [Alias (12 bits)] [Reserved (10 bits)]
```

#### 3. async_blink_esp32 Event State Machine
**Purpose**: Show event production and consumption flow
```
State: Idle
├─ Button pressed → Produce Event_1 → Return to Idle
├─ Consume Event_2 → Toggle LED → Return to Idle
└─ 1-second timer → Check state → Publish status
```

#### 4. Architecture Layers (Component Diagram)
**Purpose**: Show how async_blink_esp32 sits in OpenMRN stack
```
Application Layer (async_blink_esp32)
    ↓ uses
OpenLCB Stack (SimpleStack)
    ├─ Event Handler
    ├─ Node
    ├─ Message Transport
    └─ Alias Allocator
    ↓ uses
Transport Layer (IfTcp for WiFi)
    ↓ uses
Hardware (ESP32 WiFi radio)
```

### Medium-Priority Diagrams (Post-v0.1)

5. Alias Negotiation Flowchart (how CID/RID/AMD collision handling works)
6. Multi-Node Timeline (showing interleaved startup of multiple nodes)
7. GridConnect Protocol Format (message structure examples)
8. ESP32 CAN Wiring Schematic (for future CAN chapter)

---

## Key Unknowns to Resolve in Next Session

| Question | Impact | When? | Notes |
|----------|--------|-------|-------|
| Which ESP32 GPIO pins for button/LED? | High | T2.1 | Need to check for conflicts with OpenMRNLite; probably GPIO 4/5 or similar |
| What ESP32 board variant to recommend? | Medium | T1.2 | ESP32 DevKit v1 is common; could also mention M5Stack, Feather ESP32 |
| WiFi credential approach (hardcode vs. config file)? | Low | T2.1 | v1 approach: hardcode in .ino, mention SPIFFS for future |
| PlatformIO build flags for WiFi/TCP? | High | T2.2 | Need to verify correct flags in platformio.ini template |
| Breadboard schematic format (Mermaid vs. PNG)? | Low | T2.3 | ASCII art probably fine; PNG export if needed later |
| JMRI TCP default port and protocol details? | Medium | T2.4 | Check OpenMRN IfTcp docs; likely port 5550 |

---

## Session Notes & Context Handoff

**Project Context**:
- User has existing book skeleton with intro + startup sequence docs
- Goal: Expand to ESP32 + Arduino focus with WiFi transport (CAN deferred)
- Timeline: Multiple sessions with possible sub-agent research
- Audience: Embedded C++ developers wanting to build LCC nodes

**Design Decisions Locked**:
- ✅ PlatformIO + Arduino for toolchain
- ✅ WiFi/TCP for v1 (CAN in future chapters)
- ✅ Breadboard button + LED for hands-on I/O
- ✅ Mermaid diagrams (VS Code + mdBook support)
- ✅ JMRI for monitoring/verification
- ✅ Archive Nucleo/STM32 content (v1)

**Next Session Should**:
1. Start with T1.1-T1.3 (restructure content, remove Nucleo)
2. Create ESP32 chapter skeleton (T1.2)
3. Begin async_blink_esp32 sketch (T2.1)

---

## OpenMRNLite Local Source (NEWLY AVAILABLE)

**Location**: `OpenMRNLite/` (cloned to workspace 2025-12-18)

**Key Directories**:
- `OpenMRNLite/examples/ESP32/` - ESP32-specific examples (examine these to learn patterns!)
- `OpenMRNLite/src/` - Library source code
- `OpenMRNLite/library.json` - PlatformIO package manifest
- `OpenMRNLite/library.properties` - Arduino IDE library properties

**Proper Usage (Per OpenMRNLite README)**:

The OpenMRNLite README outlines 3 ways to use the library:
1. **Arduino Library Manager** - Download released version (recommended for users)
2. **GitHub ZIP Import** - Download released source ZIP and import into Arduino IDE
3. **Direct from source** - Export/symlink from local source tree

**For This Book Project**:
- ✅ Use local examples (`OpenMRNLite/examples/ESP32/`) as REFERENCE for learning patterns
- ✅ Example code (async_blink_esp32.ino) should use Library Manager or GitHub ZIP approach in instructions
- ✅ PlatformIO configuration can reference the local library or remote package
- ⚠️ Do NOT hardcode local paths in book examples (keep them portable)

**This Simplifies**:
- ✅ No need to search documentation; real working code is here
- ✅ Can examine how Esp32WiFiManager works, event patterns, etc.
- ✅ Can use local OpenMRNLite/library.json to verify PlatformIO config
- ✅ Sub-agent research (SA3) becomes optional; examine examples directly

**Key References from README**:
- Esp32WiFiManager for WiFi/SSID connection (creates automatic GridConnect TCP uplink)
- ESP32IOBoard example shows CDI configuration and WiFi patterns
- GPIO restrictions: avoid 6-11 (flash), 0/2 (boot), 12 (SDIO voltage), 15/5 (debug)
- Transceiver guidance: SN65HVD230 (3.3V) or MCP2551 (5V) for future CAN chapter
- Power requirements: 750mA minimum, use buck converter for CAN bus power

---

**Last Updated**: 2025-12-18  
**Research Completed By**: Initial research session + OpenMRNLite clone  
**Next Research Needed**: PlatformIO specifics, JMRI TCP configuration, GPIO conflict checks
