# Book Chapters & Content Status

## Current Table of Contents (from book.toml)

```
- Chapter 1: "Introduction" (intro.md)
  - Node
  - CAN
  - Async Blink Initialization
  - Async Blink Events
- Chapter 2: "Getting Started" (start.md)
- Chapter 3: "Getting Started with OpenMRN" (openmrn.md) ← ARCHIVE THIS
```

---

## Proposed v0.1 Table of Contents

```
- Chapter 1: "Introduction" (intro.md) [UPDATED]
  - Node
  - CAN
  - Async Blink Initialization
  - Async Blink Events
  └─ Includes: Mermaid startup sequence + event state machine diagrams

- Chapter 2: "Getting Started" (start.md) [UPDATED]
  - Overview of what we're building
  - Add: "Monitoring with JMRI (TCP)" section
  - Target audience and prerequisites

- Chapter 3: "ESP32 with Arduino & PlatformIO" (esp32-arduino.md) [NEW]
  - PlatformIO Installation & Setup
  - ESP32 Board Selection & Configuration
  - OpenMRNLite Library Installation
  - Hardware Setup (breadboard, button, LED, wiring diagram)
  - Building async_blink_esp32
  - Deploying to ESP32
  - Verification & Troubleshooting (JMRI connection, GPIO issues, etc.)
  - What's Next (preview of future topics)

- (Optional) Appendix: "Archived Content" or link to archive/openmrn-stm32.md
  - STM32 Nucleo setup (preserved for reference, not recommended for new users)
```

---

## Content Status by Chapter

### Chapter 1: Introduction (intro.md) — RESTRUCTURE NEEDED

**v0.1 Approach: HIGH-LEVEL OVERVIEW FIRST**

The current intro.md jumps into implementation details (alias negotiation). v0.1 should provide conceptual overview with forward references to future chapters for deep dives.

**Restructured Flow**:

| Section | Current Status | v0.1 Target | Notes |
|---------|---|---|---|
| Purpose & Scope | ✅ Complete | ✅ Keep | Why this book, focus on creating LCC products |
| Node Concept | ✅ Complete | ✅ Keep | Nodes as building blocks, 6-byte unique IDs |
| **Transport Layers** (NEW) | ⏳ Missing | 🚨 ADD | High-level: CAN vs WiFi/TCP; why both exist; WiFi first in v0.1, CAN later |
| **Startup Sequence Overview** (RESTRUCTURED) | 🔄 Partial | 🚨 REFACTOR | What happens (CID → RID → AMD → Init Complete) without collision detection details; reference future chapter for how |
| **Run Mode Overview** (NEW) | ⏳ Missing | 🚨 ADD | Nodes online, producing/consuming events, responding to other nodes |
| **Multi-Node Participation** (NEW) | ⏳ Missing | 🚨 ADD | What other nodes do when one node starts (listening, responding with info) |
| **Event Basics** | ✅ Complete | 🔄 Expand | Keep existing; add Mermaid state machine diagram |
| **Standards References** | ✅ Complete | ✅ Keep | Link to S-9.7.x.x documents for detailed specs |
| **Forward References** (NEW) | ⏳ Missing | 🚨 ADD | "See Chapter X: Understanding Startup Sequence" for alias negotiation details, etc. |

**Key Principle**: Explain WHAT, then point to future chapters for HOW

### Chapter 2: Getting Started (start.md)

| Section | Current Status | v0.1 Target | Notes |
|---------|---|---|---|
| Overview | ✅ Complete | ✅ Keep | High-level intro to what we're building |
| Nucleo Hardware | ✅ Complete | 📦 Archive | Move to archive folder; keep reference link |
| CAN Configuration | ⚠️ Partial | 📦 Archive | Specific to Nucleo/STM32; defer CAN to later chapter |
| JMRI Monitoring | ✅ Complete (for Nucleo) | 🔄 Update | Generalize to TCP/ESP32 setup; create separate "Monitoring with JMRI (TCP)" section |
| Custom Applications | ✅ Complete | 📦 Archive | Directory structure is general, but keep with Nucleo docs |
| **MISSING** | | 🚨 ADD | Updated prerequisites for ESP32 + PlatformIO |

### Chapter 3: ESP32 with Arduino & PlatformIO (esp32-arduino.md)

| Section | Current Status | v0.1 Target | Notes |
|---|---|---|---|
| PlatformIO Installation | ⏳ New | 🔄 Write | Step-by-step setup guide |
| ESP32 Board Selection | ⏳ New | 🔄 Write | Recommended boards (ESP32 DevKit v1, etc.) |
| Library Installation | ⏳ New | 🔄 Write | OpenMRNLite via Arduino Library Manager or source |
| Hardware Setup | ⏳ New | 🔄 Write | GPIO pins, button/LED circuit, breadboard layout, BOM, wiring diagram |
| Building async_blink_esp32 | ⏳ New | 🔄 Write | platformio.ini template, project structure, build commands |
| Deploying to ESP32 | ⏳ New | 🔄 Write | Serial port selection, upload process, debugging output |
| Verification & Troubleshooting | ⏳ New | 🔄 Write | Expected startup messages, event verification, common issues |
| What's Next | ⏳ New | 🔄 Write | Preview CAN hardware, advanced events, memory config |

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

- **Chapter 4: Adding CAN Hardware** - MCP2551/SN65HVD23x transceivers, wiring, configuration
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
