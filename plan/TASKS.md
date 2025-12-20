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

### T2.0 - Write PlatformIO & OpenMRNLite Installation Guide
- **Status**: ✅ Completed
- **Priority**: HIGH
- **Effort**: 1.5 hours
- **Description**:
  - Add content to `esp32-arduino.md` for first 3 sections:
    - **PlatformIO Installation & Setup**: Step-by-step VS Code + PlatformIO extension installation, verify installation ✅
    - **ESP32 Board Selection**: Recommended boards (ESP32 DevKit v1, etc.), ESP32 platform installation in PlatformIO ✅
    - **OpenMRNLite Library Installation**: Library Manager method (simplified for v0.1), verification steps ✅
  - Included command examples and code snippets ✅
  - Linked to official documentation for deeper details ✅
  - Readers can complete setup before moving to T2.1/T2.2 ✅
- **Owner**: Completed 2025-12-19
- **Depends On**: T1.4
- **Blocks**: T2.1, T2.2

### T2.1 - Create async_blink_esp32 Arduino Sketch
- **Status**: ⏳ Not Started
- **Priority**: HIGH
- **Effort**: 2-3 hours
- **Description**:
  - Create `async_blink_esp32.ino` sketch featuring:
    - WiFi connection setup (SSID/password hardcoded for v1)
    - OpenMRNLite SimpleStack with TCP transport
    - GPIO button input (detect press, produce event)
    - GPIO LED output (consume event, toggle LED)
    - Serial debug output showing startup and events
    - Two fixed event IDs (one for button, one for LED)
  - Include detailed inline comments explaining startup sequence and event handling
  - **NEW**: Use `OpenMRNLite/examples/ESP32/` as reference (now locally available!)
  - Test on actual ESP32 hardware
- **Owner**: Next session (sub-agent research SA3 now OPTIONAL)
- **Depends On**: T2.0
- **Blocks**: T2.3, T4.1

### T2.2 - Create PlatformIO Configuration Template
- **Status**: ⏳ Not Started
- **Priority**: HIGH
- **Effort**: 1 hour
- **Description**:
  - Create `platformio.ini` template with:
    - ESP32 board selection (e.g., esp32doit-devkit-v1)
    - Arduino framework
    - OpenMRNLite library dependency (per README: use Library Manager or GitHub source)
    - Build flags for WiFi + TCP
    - Serial monitor configuration (baud rate, port auto-detection)
    - Upload speed and protocol settings
  - Include inline comments for customization
  - Document required edits (WiFi SSID, password)
  - Document library installation options (Arduino Library Manager, GitHub ZIP, or local source)
  - Include in `esp32-arduino.md` as code block + downloadable file
- **Owner**: Next session
- **Depends On**: T2.0
- **Blocks**: T4.1

### T2.3 - Document Breadboard Circuit (Button + LED)
- **Status**: ⏳ Not Started
- **Priority**: HIGH
- **Effort**: 1.5 hours
- **Description**:
  - Create section in `esp32-arduino.md`: "Hardware Setup"
  - GPIO pin assignment (recommend safe, accessible pins)
  - Button wiring: GPIO pin → Button → GND (with pull-up config)
  - LED wiring: GPIO pin → Resistor (220Ω) → LED → GND
  - BOM: ESP32, breadboard, tactile button, LED, resistors, jumpers
  - Create Mermaid diagram or ASCII schematic showing connections
  - Include photo reference or detailed text description
- **Owner**: Next session
- **Depends On**: T1.2
- **Blocks**: T2.1, T4.1

### T2.4 - Create Detailed JMRI TCP Setup Guide
- **Status**: ⏳ Not Started
- **Priority**: HIGH
- **Effort**: 1.5 hours
- **Description**:
  - Add section to `esp32-arduino.md`: "Verification with JMRI"
  - Step-by-step JMRI configuration (detailed):
    - Download/install JMRI
    - New Connection → LCC Interface → GridConnect
    - TCP settings (localhost:12021 or ESP32 IP address)
    - Connection verify steps
  - Show expected output: startup messages (CID/RID/AMD/Init Complete)
  - Show event production/consumption in JMRI console
  - Troubleshooting: connection refused, no messages, etc.
  - Include sample console output screenshot/transcript
- **Owner**: Next session
- **Depends On**: T2.1
- **Blocks**: T4.1

---

## Phase 3: Diagrams & Visualization

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
