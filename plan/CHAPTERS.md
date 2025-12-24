# Book Chapters & Content Status

**Last Updated**: 2025-12-21  
**Current Status**: Phase 0 & 1 Complete, Phase 2 In Progress (T2.0.1 - CDI Enhancement)

## Current Table of Contents (from SUMMARY.md)

```
- Chapter 1: "Introduction" (src/intro.md)
- Chapter 2: "Getting Started" (src/02-start/start.md)
- Chapter 3: "ESP32 with Arduino & PlatformIO" (src/03-esp32-arduino/esp32-arduino.md)
```

## Archived Content
- archive/openmrn-stm32.md (Nucleo/STM32 OpenMRN setup)
- archive/start-nucleo.md (DevKit hardware information)

---

## Planned Future Chapters (unordered)

- Configuration & Persistence (configuration-persistence.md)
  - CDI and ACDI explanation (SNIP vs ACDI)
  - Configuration storage (SPIFFS, factory_reset), `apply_configuration()` pattern
  - How JMRI discovers and edits configuration (LccPro vs DecoderPro notes)

- Switching to CAN (switching-to-can.md)
  - Transport differences (WiFi/TCP vs CAN)
  - CAN transceiver choices, wiring, termination, platform flags
  - Migration notes for async_blink example

- Physical I/O / GPIO hardware (gpio-hardware.md)
  - Breadboard wiring, BOM, pin assignments
  - Breadboard schematic and recommended pins for common ESP32 boards

- Understanding Startup Sequence (detailed)
  - CID/RID/AMD negotiation, collision handling, timeouts

- Advanced topics: Node Configuration, Advanced Events, CAN integration, etc.

---

## Content Status by Chapter

### Chapter 1: Introduction (intro.md) — ✅ RESTRUCTURED

**v0.1 Approach: HIGH-LEVEL OVERVIEW** (COMPLETED 2025-12-19)

The introduction has been restructured from implementation-focused to concept-focused with forward references to future chapters.

**Completed Updates**:

| Section | Status | Notes |
|---------|--------|-------|
| Purpose & Scope | ✅ Complete | Why this book, focus on creating LCC products |
| Focus of this book | ✅ Updated | WiFi/TCP first, CAN later approach |
| Node Concept | ✅ Complete | Nodes as building blocks, 6-byte unique IDs |
| **Transport Layers** | ✅ Added | CAN vs WiFi/TCP rationale; WiFi first in v0.1 |
| **Node Startup Sequence** | ✅ Restructured | WHAT happens (CID → RID → AMD → Init Complete) with Mermaid diagram |
| **Multi-Node Participation** | ✅ Added | Other nodes listen/respond during startup |
| **Events and Run Mode** | ✅ Restructured | Producer/consumer model with Mermaid event flow diagram |
| **Standards References** | ✅ Complete | Links to S-9.7.x.x documents |
| **Forward References** | ✅ Tracked | See plan/FORWARD_REFERENCES.md |

**Key Principle Achieved**: Explains WHAT conceptually, points to future chapters for HOW

### Chapter 2: Getting Started (start.md) — ✅ REWRITTEN

### Chapter 2: Getting Started (start.md) — ✅ REWRITTEN

**Completed Updates** (2025-12-19):

| Section | Status | Notes |
|---------|--------|-------|
| What We're Building | ✅ Complete | Overview of button → LED example with WiFi |
| Why WiFi/TCP First? | ✅ Added | Transport rationale, reinforces Chapter 1 |
| Platform Choice: ESP32 | ✅ Added | Why ESP32, CAN capability, other platforms mentioned |
| Prerequisites & Assumptions | ✅ Added | Hardware BOM, knowledge requirements |
| Development Environment Overview | ✅ Added | PlatformIO, Arduino framework, OpenMRNLite |
| Monitoring with JMRI (TCP) | ✅ Added | High-level overview, GridConnect protocol |
| What's Next | ✅ Added | Preview of Chapter 3 |
| **Nucleo Hardware** | ✅ Archived | Moved to archive/start-nucleo.md |
| **CAN Configuration** | ✅ Archived | Moved to archive/start-nucleo.md |

### Chapter 3: OpenMRN-Lite Architecture & Capabilities (openmrn-architecture.md) — ✅ COMPLETED

**v0.1 Approach: ARCHITECTURAL CLARITY BEFORE IMPLEMENTATION** (COMPLETED 2025-12-21)

This chapter clarifies the relationship between OpenMRN (full version) and OpenMRN-Lite (Arduino version), answering common questions before readers dive into the ESP32 chapter (now Chapter 6).

**Completed Content**:

| Section | Status | Notes |
|---------|--------|-------|
| What is OpenMRN-Lite? | ✅ Complete | Explains it's the Arduino version, not a "lite" compromise |
| Why it's the Right Choice | ✅ Complete | Single-threaded, Arduino-native, supports CDI/events/datagrams |
| What OpenMRN-Lite Does NOT Support | ✅ Complete | Virtual nodes, traction, bridging (require FreeRTOS) |
| When to Use Each | ✅ Complete | Decision matrix for OpenMRN-Lite vs full OpenMRN with ESP-IDF |
| Configuration & Learning Implications | ✅ Complete | CDI support, why configuration can be hardcoded in v0.1 |
| Looking Ahead | ✅ Complete | Preview of Chapter 3 and future topics |

**Key Learning Achieved**: Readers understand the toolchain choices and why OpenMRN-Lite is the only option for Arduino/ESP32, not a stepping stone.

**Cross-References**:
- Referenced from Chapter 2 (Getting Started) - new section "Understanding OpenMRN-Lite"
- Referenced from Chapter 6 (ESP32 Arduino) - Overview section mentions "Chapter 3 for deeper dive"
- Tracked in FORWARD_REFERENCES.md

### Chapter 6: ESP32 with Arduino & PlatformIO (esp32-arduino.md) — 🔄 95% COMPLETE (WiFi-Only Focus)

| Section | Current Status | v0.1 Target | Notes |
|---|---|---|---|
| Overview | ✅ Complete | ✅ Done | Software-only + optional hardware structure |
| PlatformIO Installation | ✅ Complete | ✅ Done | Step-by-step setup guide |
| ESP32 Board Selection | ✅ Complete | ✅ Done | Recommended boards (ESP32 DevKit v1, etc.) |
| Library Installation | ✅ Complete | ✅ Done | OpenMRNLite via Arduino Library Manager or source |
| Creating async_blink_esp32 | ✅ Complete | ✅ Done | Full code with inline comments, tested & validated |
| Code Walkthrough | ✅ Complete | ✅ Done | Detailed explanation of each section (WiFi, OpenMRN, TCP) |
| Building & Uploading | ✅ Complete | ✅ Done | PlatformIO build and firmware upload steps |
| Serial Monitor Verification | ✅ Complete | ✅ Done | Expected output, verification steps, troubleshooting |
| JMRI Configuration | ✅ Complete | ✅ Done | TCP connection setup, message monitoring, traffic interpretation |
| Physical Hardware Section | ⏳ Moving to Ch6 | 🔄 Deferred | Moving to gpio-hardware.md (Chapter 6, future release) |
| Troubleshooting | ✅ Complete | ✅ Done | Library, USB detection, WiFi, JMRI issues |
| What's Next | ✅ Complete | ✅ Done | References Chapter 6 (Physical I/O) for future hardware integration |

**Status Summary**: Chapter 6 is now focused purely on WiFi async blink (Sections 1-9). Only remaining task is T2.3: move incomplete "Physical Hardware (Optional)" section to Chapter 6 (gpio-hardware.md).

---

## Related Files & Resources to Reference

| Resource | Location | Usage in Book |
|---|---|---|
| async_blink (desktop reference) | `openmrn/applications/async_blink/` | Conceptual reference; explain how ESP32 version adapts it |
| OpenMRNLite library | `openmrn/arduino/OpenMRNLite/` | Document API and configuration in Chapter 3 |
| Technical Standards | `markdown/standards/` | Reference in diagrams, link for deep dives |
| Node Startup docs | Current intro.md | Leverage existing content in Chapter 3 verification section |

---

## Content Dependencies & Flow

```
Chapter 1 (Intro)
├─ Node basics
├─ CAN protocol (conceptual)
└─ Async Blink theory (initialization + events)
    └─ [Mermaid Diagrams Help Explain This]

Chapter 2 (Getting Started)
├─ What we're building (updated for ESP32)
├─ Prerequisites & assumptions
└─ Monitoring with JMRI (TCP)
    └─ [Needed for Chapter 3 verification]

Chapter 3 (ESP32 Arduino)
├─ Requires understanding from Chapter 1 & 2
├─ PlatformIO setup
├─ Hardware wiring
├─ Building async_blink_esp32
│   └─ [Practical application of Chapter 1 concepts]
├─ Verification with JMRI
│   └─ [Uses Chapter 2 JMRI setup]
└─ What's Next preview
```

---

## Writing Principles for v0.1

1. **Simplicity First**: Explain WiFi/TCP as intentional stepping stone to CAN; don't try to explain both
2. **Hands-On**: Every section should have concrete steps or code
3. **Working Example**: async_blink_esp32 should be fully buildable and runnable
4. **Diagrams Aid Learning**: Use Mermaid to visualize abstract concepts (startup sequence, event flow)
5. **Verify Early**: Troubleshooting section helps readers confirm success quickly
6. **Link to Standards**: Reference markdown/standards/ for deep dives without overwhelming v0.1

---

## Post-v0.1 Chapters (Future)

These topics are NOT included in v0.1 but are listed for future planning:

 - **Adding CAN Hardware** - MCP2551/SN65HVD23x transceivers, wiring, configuration
- **Chapter 5: Understanding Events in Depth** - Event ID generation, producer/consumer patterns, CDI
- **Chapter 6: Node Configuration & SNIP** - Simple Node Information Protocol
- **Chapter 7: Memory Configuration** - Configuration space, reading/writing parameters
- **Chapter 8: Train Control & Advanced Features** - Traction protocol, broadcast time
- **Appendix A: OpenMRN Stack Deep Dive** - NodeInitializeFlow, SimpleStack architecture
- **Appendix B: Troubleshooting & FAQ**
- **Appendix C: Archived Nucleo/STM32 Content**

---

**Last Updated**: 2025-12-18  
**Status**: Planning phase; ready for content creation in next session
