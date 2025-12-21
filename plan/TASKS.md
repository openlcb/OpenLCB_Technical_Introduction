# Work Tasks - ESP32 WiFi Foundation

## Phase 0: Conceptual Restructuring (NEW)

### T0.1 - Restructure intro.md for High-Level Overview
- **Status**: ✅ Completed
- **Priority**: HIGH (blocks other tasks)
- **Effort**: 2 hours
- **Description**:
  - **Goal**: Move from implementation-focused to concept-focused introduction
  - Reorganize sections to flow:
    1. Purpose & Scope (existing, keep) ✅
    2. Node Concept (existing, keep) ✅
    3. **NEW: Transport Layers Overview** ✅ - Explain CAN vs WiFi/TCP at high level; establish why WiFi first in v0.1
    4. **REFACTOR: Startup Sequence Overview** ✅ - WHAT happens (CID → RID → AMD → Init Complete) without collision detection mechanics; add Mermaid diagram; forward reference to future chapters
    5. **NEW: Run Mode Overview** ✅ - What happens after startup (nodes online, producing/consuming events)
    6. **NEW: Multi-Node Participation** ✅ - Other nodes listen/respond during startup; explain participation without technical details
    7. Event Basics (existing, enhance with diagram) ✅
    8. Standards References (existing, keep) ✅
  - Add forward references to future chapters (track in plan/FORWARD_REFERENCES.md) ✅
  - Updated SUMMARY.md chapter titles for clarity ✅
  - Configured and installed mdbook-mermaid for diagram support ✅
- **Owner**: Completed 2025-12-19
- **Depends On**: None
- **Blocks**: T2.1, T2.2, T3.1, other content tasks
- **Related**: plan/FORWARD_REFERENCES.md (tracking file for link updates)

---

## Phase 1: Content Restructuring & Preparation

### T1.1 - Archive Nucleo/STM32 Content
- **Status**: ✅ Completed
- **Priority**: HIGH
- **Effort**: 1 hour
- **Description**: 
  - Create `src/archive/` folder ✅
  - Move `src/openmrn.md` to `src/archive/openmrn-stm32.md` ✅
  - Removed from SUMMARY.md (mdBook uses SUMMARY.md, not book.toml for TOC) ✅
  - Skipped updating start.md per user preference (no STM32 mentions)
- **Owner**: Completed 2025-12-19
- **Depends On**: None
- **Blocks**: T2.1, T2.2

### T1.2 - Create Book Chapter Skeleton for "ESP32 with Arduino & PlatformIO"
- **Status**: ✅ Completed
- **Priority**: HIGH
- **Effort**: 30 minutes
- **Description**:
  - Create new file: `src/esp32-arduino.md` with outline sections: ✅
    - PlatformIO Installation & Setup
    - ESP32 Board Selection
    - OpenMRNLite Library Installation
    - Hardware Setup (breadboard circuit)
    - Building async_blink_esp32
    - Deploying to ESP32
    - Verification & Troubleshooting
    - What's Next
  - Add to SUMMARY.md after `start.md` ✅
- **Owner**: Completed 2025-12-19
- **Depends On**: T1.1
- **Blocks**: T2.2, T3.1

### T1.3 - Update SUMMARY.md Table of Contents
- **Status**: ✅ Completed
- **Priority**: HIGH
- **Effort**: 15 minutes
- **Description**:
  - Remove `openmrn.md` chapter ✅
  - Add new `esp32-arduino.md` chapter as Chapter 3 ✅
  - Updated chapter titles for clarity ("Node Startup Sequence", "Events and Run Mode") ✅
  - Verify build with `mdbook build` ✅
  - Configured mdbook-mermaid for diagram support ✅
- **Owner**: Completed 2025-12-19
- **Depends On**: T1.1, T1.2
- **Blocks**: None

### T1.4 - Rewrite Getting Started Chapter for ESP32 Focus
- **Status**: ✅ Completed
- **Priority**: HIGH
- **Effort**: 1.5 hours
- **Description**:
  - Archive old `start.md` to `archive/start-nucleo.md` ✅
  - Complete rewrite of `start.md` with new structure: ✅
    - What We're Building (button → LED example overview)
    - Why WiFi/TCP First? (transport rationale, reinforces Chapter 1)
    - Platform Choice: ESP32 (why ESP32, CAN capability, other platforms)
    - Prerequisites & Assumptions (hardware BOM, knowledge requirements)
    - Development Environment Overview (PlatformIO, Arduino, OpenMRNLite)
    - Monitoring with JMRI (TCP) (high-level overview, GridConnect protocol)
    - What's Next (preview of Chapter 3)
  - Bridges conceptual Introduction with hands-on ESP32 chapter ✅
- **Owner**: Completed 2025-12-19
- **Depends On**: T1.3
- **Blocks**: T2.4

---

## Phase 2: Example Code & Hardware Documentation

### T2.0 - Write PlatformIO & OpenMRN Installation Guide
- **Status**: ✅ Completed (OpenMRNLite) → ✅ COMPLETED (Enhanced with CDI via T2.0.1)
- **Priority**: HIGH
- **Effort**: 1.5 hours
- **Description**:
  - Add content to `esp32-arduino.md` for first 3 sections:
    - **PlatformIO Installation & Setup**: Step-by-step VS Code + PlatformIO extension installation, verify installation ✅
    - **ESP32 Board Selection**: Recommended boards (ESP32 DevKit v1, etc.), ESP32 platform installation in PlatformIO ✅
    - **OpenMRN Library Installation**: OpenMRN-Lite with CDI support (no library change needed)
  - Included command examples and code snippets ✅
  - Linked to official documentation for deeper details ✅
  - Readers can complete setup before moving to T2.1/T2.2 ✅
  - **STRATEGIC CLARIFICATION**: OpenMRN-Lite IS the Arduino version (single-threaded, no RTOS). Full OpenMRN requires FreeRTOS and cannot run in Arduino. No library migration needed; just enhance CDI support.
    - CDI support enables JMRI-based configuration (no recompiling for event ID changes)
    - Minimal code complexity increase (~50 lines in config.h, net zero in main.cpp)
    - Prevents "hit a wall" where learners outgrow OpenMRNLite limitations
    - Students learn with production tools they'll actually use
- **Owner**: Completed 2025-12-19 (OpenMRNLite), Updating 2025-12-21
- **Depends On**: T1.4
- **Blocks**: T2.0.1, T2.1, T2.2

### T2.0.1 - Enhance async_blink_esp32 with CDI support (NEW - IMMEDIATE TASK)
- **Status**: ✅ COMPLETED (2025-12-21)
- **Priority**: HIGH (blocks rest of Chapter 3)
- **Effort**: 1.5 hours (code changes + hardware testing)
- **Architecture Note**: OpenMRN-Lite IS the Arduino version of OpenMRN (single-threaded, no RTOS). Full OpenMRN requires FreeRTOS/POSIX and cannot run in Arduino. OpenMRN-Lite already supports CDI. No library migration needed.
- **Description**:
  1. ✅ Keep `test/async_blink_esp32/platformio.ini`: OpenMRN-Lite library unchanged
  2. ✅ Rewrite `test/async_blink_esp32/include/config.h`: Enable minimal CDI with Acdi + UserInfoSegment (from IOBoard pattern)
  3. ✅ Rewrite `test/async_blink_esp32/src/main.cpp`: Enable CDI generation, keep hardcoded WiFi + event IDs
  4. ✅ Test on ESP32 DevKit hardware: Build, flash, verify serial output, verify JMRI sees events and can view CDI
  - **Result**: ✅ async_blink_esp32 using OpenMRN-Lite with CDI enabled and configuration exposed (but hardcoded for now)
  - **Accomplishments**:
    - Fixed SNIP byte offsets (name at offset 1, description at offset 64) → JMRI displays node name correctly
    - Refactored setup() into 5 focused initialization functions (serial, filesystem, network, OpenLCB, TCP)
    - Moved configuration constants (SNIP_NODE_NAME, SNIP_NODE_DESC) to config.h for centralization
    - Removed dead code (unused CDI_FILENAME constant)
    - Verified compilation: No errors, 935KB firmware (71.4% flash), 14% RAM usage
  - **Deferred to Chapter 5**: WiFiConfiguration, factory reset, configurable event IDs
- **Owner**: Completed 2025-12-21
- **Depends On**: T2.0
- **Blocks**: T2.1, T2.2, T2.6 (documentation update)

### T2.1 - Create async_blink_esp32 Arduino Sketch (ENHANCED WITH CDI)
- **Status**: ✅ COMPLETED (OpenMRNLite) → 🔄 UPDATING (CDI Enhancement via T2.0.1)
- **Priority**: HIGH
- **Effort**: 2-3 hours (original), 30 min (CDI enhancement via T2.0.1)
- **Description**:
  - **Original work (2025-12-20)**: WiFi setup, OpenMRN-Lite TCP transport, event production loop, tested on hardware ✅
  - **Enhancement**: Enable CDI in config.h (T2.0.1 handles this), re-test to verify identical behavior
  - **Note**: Physical GPIO deferred to Chapter 4 (WiFi-only async_blink for v0.1)
- **Owner**: Completed 2025-12-20 (OpenMRN-Lite), Enhancing via T2.0.1
- **Depends On**: T2.0.1 (CDI-enabled code working)
- **Blocks**: T2.3, T4.1

### T2.2 - Create PlatformIO Configuration Template (CDI-ENABLED OPENMRN-LITE)
- **Status**: ✅ COMPLETED (OpenMRNLite) → 🔄 UPDATING (CDI Enhancement via T2.0.1)
- **Priority**: HIGH
- **Effort**: 1 hour
- **Description**:
  - **Original work**: Created `platformio.ini` template with OpenMRN-Lite dependency ✅
  - **Enhancement work** (via T2.0.1):
    - Keep library dependency as `openmrn/OpenMRNLite` (no changes needed) ✅
    - Add documentation comment about CDI support for future reference ✅
    - Document OpenMRN-Lite capabilities ✅
  - Working example in async_blink_esp32 project, tested and validated on hardware ✅
  - Include in `esp32-arduino.md` as code block with explanation
- **Owner**: Completed 2025-12-20, Enhancing via T2.0.1
- **Depends On**: T2.0.1
- **Blocks**: Chapter 3 documentation

### T2.3 - Move Physical I/O Content to Chapter 4 (gpio-hardware.md)
- **Status**: ⏳ PENDING
- **Priority**: MEDIUM (defers hardware to v0.2, refocuses Chapter 3 on WiFi-only)
- **Effort**: 30 minutes
- **Description**:
  - Remove 'Adding Physical Hardware (Optional)' section from esp32-arduino.md ✓ Identified
    - GPIO Pin Assignment (GPIO 12 button, GPIO 13 LED) → Move to gpio-hardware.md
    - Bill of Materials (breadboard, button, LED, resistor, jumpers) → Move to gpio-hardware.md
  - Update gpio-hardware.md with moved content
  - Modify esp32-arduino.md 'What's Next' section to reference Chapter 4 for hardware integration
  - **Result**: Chapter 3 focused purely on WiFi async blink (no hardware setup needed)
- **Owner**: After T2.0.1 verified working
- **Depends On**: T2.0.1 (code working on hardware)
- **Blocks**: Chapter 4 completion

### T2.4 - Create Detailed JMRI TCP Setup Guide (NO CHANGES NEEDED)
- **Status**: ✅ COMPLETED
- **Priority**: HIGH
- **Effort**: 1.5 hours
- **Description**:
  - Add section to `esp32-arduino.md`: "Verification with JMRI" ✅
  - Step-by-step JMRI configuration (detailed): ✅
    - Download/install JMRI
    - New Connection → LCC Interface → GridConnect
    - TCP settings (localhost:12021 or ESP32 IP address)
    - Connection verify steps
  - Show expected output: startup messages (CID/RID/AMD/Init Complete) ✅
  - Show event production/consumption in JMRI console ✅
  - Troubleshooting: connection refused, no messages, etc. ✅
  - Include sample console output screenshot/transcript ✅
  - **Note**: Works identically with full OpenMRN (TCP Hub behavior unchanged) ✅
- **Owner**: Completed 2025-12-19
- **Depends On**: T2.1 ✅
- **Blocks**: T4.1

### T2.5 - Make WiFi Configuration & Event IDs Configurable (DEFERRED)
- **Status**: 📋 PLANNED for Chapter 5
- **Priority**: MEDIUM (advanced topic)
- **Effort**: 2-3 hours
- **Description**:
  - Update `test/async_blink_esp32/include/config.h`:
    - Add `WiFiConfiguration` to CDI segment
    - Add configurable event ID entries in SNIP data segment
  - Update `test/async_blink_esp32/src/main.cpp`:
    - Use `Esp32WiFiManager` to read WiFi credentials from CDI
    - Read event IDs from configuration memory instead of hardcoded constants
  - Update Chapter 3 / Chapter 5 documentation:
    - Show how to use JMRI CDI editor to reconfigure WiFi and events
    - Explain config persistence across reboots
  - **Result**: Node can be reconfigured via JMRI without reflashing firmware
- **Owner**: TBD (Chapter 5 session)
- **Depends On**: T2.0.1
- **Blocks**: None (enhancement for v0.2)

---

## Phase 2.5: Library Strategy & Documentation (NEW)

### T2.6 - Update Chapter 3 Documentation for OpenMRN-Lite CDI
- **Status**: ✅ COMPLETED (2025-12-21)
- **Priority**: HIGH
- **Effort**: 2 hours
- **Description**:
  - Updated [src/esp32-arduino.md](src/esp32-arduino.md) sections:
    - **config.h**: Updated code block with SNIP_NODE_NAME, SNIP_NODE_DESC, Acdi, UserInfoSegment ✅
    - **main.cpp**: Updated with refactored helper functions (init_serial, init_filesystem, init_network, init_openlcb_stack, init_tcp_hub) ✅
    - **Explanation text**: Added descriptions of CDI, SNIP Static/Dynamic Data, and persistence note ✅
    - **Code Walkthrough**: 
      - Added intro paragraph explaining code organization ✅
      - Section 3: Removed CDI_FILENAME/CDI_DATA references, kept brief ✅
      - Section 4: Expanded SPIFFS/SNIP explanation with byte offsets and future chapter notes ✅
      - Sections 5-9: Updated to explain helper functions and their responsibilities ✅
      - Section 6: Added init_snip_user_data() explanation and TCP Hub coverage ✅
      - Sections 7-9: Added new sections covering TCP Hub, setup(), and event loop ✅
    - **Result**: Chapter 3 documentation matches working OpenMRN-Lite with CDI code ✅
- **Owner**: Completed 2025-12-21
- **Depends On**: T2.0.1 (code working) ✅
- **Blocks**: None

### T2.6.1 - Create OpenMRN-Lite Architecture Chapter (Chapter 2.5)
- **Status**: 📋 PLANNED (new chapter 2.5)
- **Priority**: MEDIUM (informative, not blocking)
- **Effort**: 1.5 hours
- **Description**:
  - **Create new file**: `src/libraries-openmrn-comparison.md` (will become Chapter 2.5)
  - **Contents**:
    - Opening: "Choosing Your OpenMRN Library"
    - Why we chose full OpenMRN for this book
      - CDI support enables JMRI-based configuration
      - Prevents limitations when experimenting
      - Minimal code complexity increase
    - Comparison table:
      - Code complexity
      - Configuration workflow
      - Experimentation friction (recompile vs JMRI)
      - Production readiness
      - When to use each (hobbyists with fixed nodes → OpenMRNLite, learners/experimentation → full OpenMRN)
    - Key insight: "False simplicity trap" of OpenMRNLite
      - Looks easier but forces firmware updates for every change
      - Full OpenMRN's upfront complexity pays off immediately
    - What's deferred (factory reset, CAN, etc.) to later chapters
  - **Add to SUMMARY.md** as Chapter 2.5 (between "Getting Started" and "ESP32 with Arduino")
  - **Result**: Transparent explanation of library choice, manages expectations
- **Owner**: TBD (after T2.0.1 verified)
- **Depends On**: T2.0.1
- **Blocks**: T2.6

---\n\n## Phase 3: Diagrams & Visualization

### T3.1 - Create Mermaid: Node Startup Sequence
- **Status**: ⏳ Not Started
- **Priority**: MEDIUM
- **Effort**: 1 hour
- **Description**:
  - Mermaid sequence diagram showing:
    - ESP32 node startup flow
    - Four CID (Check ID) frames with node ID parts
    - RID (Reserve ID) message
    - AMD (Alias Map Definition) message
    - Initialize Complete message
    - Timing/pauses between messages
  - Include in `intro.md` with explanation text
  - Reference from `esp32-arduino.md` as well
- **Owner**: Next session
- **Depends On**: None
- **Blocks**: None

### T3.2 - Create Mermaid: async_blink_esp32 Event State Machine
- **Status**: ⏳ Not Started
- **Priority**: MEDIUM
- **Effort**: 45 minutes
- **Description**:
  - Mermaid state diagram showing:
    - Idle/wait state
    - Button pressed → produce event_1 → return to idle
    - Consume event_2 → toggle LED → return to idle
    - Timing (1-second loop or event-driven)
  - Include in `esp32-arduino.md` after code explanation
  - Keep simple; focus on clear event flow
- **Owner**: Next session
- **Depends On**: T2.1
- **Blocks**: None

### T3.3 - Create Optional: Breadboard Schematic Diagram
- **Status**: ⏳ Not Started
- **Priority**: LOW
- **Effort**: 1.5 hours
- **Description**:
  - Option A: ASCII art schematic in markdown
  - Option B: Mermaid diagram (if feasible for wiring)
  - Option C: PNG export from draw.io or similar tool
  - Show ESP32 GPIO pins → button/LED connections clearly
  - Include resistor values and pin labels
  - Include in `esp32-arduino.md`
- **Owner**: Defer to later session if time-constrained
- **Depends On**: T2.3
- **Blocks**: None

---

## Phase 4: Integration & Testing

### T4.1 - End-to-End Testing & Verification Guide
- **Status**: ⏳ Not Started
- **Priority**: HIGH
- **Effort**: 2 hours
- **Description**:
  - Build & deploy async_blink_esp32 on actual ESP32 hardware
  - Verify:
    - Sketch compiles with PlatformIO
    - ESP32 connects to WiFi
    - Startup messages appear in JMRI (CID/RID/AMD/Init Complete)
    - Button press produces event in JMRI console
    - JMRI-generated event toggles LED
  - Document expected serial output and JMRI messages
  - Create troubleshooting guide for common issues:
    - WiFi connection failures
    - JMRI TCP connection refused
    - Button/LED GPIO conflicts
    - Startup message delays or missing frames
  - Add to `esp32-arduino.md` as "Verification & Troubleshooting" section
- **Owner**: Next session (must have hardware)
- **Depends On**: T2.1, T2.2, T2.3, T2.4
- **Blocks**: T5.1

### T4.2 - Create "What's Next" Preview Section
- **Status**: ⏳ Not Started
- **Priority**: MEDIUM
- **Effort**: 30 minutes
- **Description**:
  - Add section to end of `esp32-arduino.md` or new file:
    - "You've built a WiFi-based LCC node! Here's what comes next:"
    - Preview: CAN Hardware Setup, Advanced Events, Memory Config, SNIP/CDI
    - Explain WiFi→CAN transition rationale
    - Link to archived Nucleo content (optional alternative)
  - Keep tone encouraging and forward-looking
- **Owner**: Next session
- **Depends On**: T4.1
- **Blocks**: None

---

## Phase 5: Book Build & Release

### T5.1 - Final Book Build & Verification
- **Status**: ⏳ Not Started
- **Priority**: HIGH
- **Effort**: 1 hour
- **Description**:
  - Run `mdbook build` and verify:
    - No build errors
    - All chapters appear in TOC
    - Mermaid diagrams render correctly
    - Links work (archive reference if included)
  - Run `mdbook serve` and spot-check HTML rendering
  - Verify async_blink_esp32 code blocks format correctly
  - Check for markdown syntax errors
- **Owner**: Final session
- **Depends On**: All Phase 1-4 tasks
- **Blocks**: None

### T5.2 - Create Release Notes / Session Summary
- **Status**: ⏳ Not Started
- **Priority**: MEDIUM
- **Effort**: 30 minutes
- **Description**:
  - Update PROJECT_STATUS.md: mark v0.1 complete
  - Document what was accomplished:
    - Chapters added/moved
    - Examples created
    - Diagrams added
  - List what's NOT included (CAN, advanced features) for future chapters
  - Note any open questions or TODOs for next iteration
- **Owner**: Final session
- **Depends On**: T5.1
- **Blocks**: None

---

## Work Prioritization Summary

**If time is limited, prioritize in this order:**

1. **Must Have (Blocking)**: T1.1, T1.2, T2.1, T2.3, T2.4, T4.1
2. **Should Have (High Value)**: T3.1, T3.2, T1.3
3. **Nice to Have (Polish)**: T3.3, T4.2, T5.2
4. **Future**: CAN hardware, advanced features

---

**Last Updated**: 2025-12-18  
**Next Review**: Start of next session before picking tasks
