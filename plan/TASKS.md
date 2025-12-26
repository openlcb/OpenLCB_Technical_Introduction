# Work Tasks - ESP32 WiFi Foundation

> Canonical backlog: `TASKS.md` records completed, in-progress, and planned work. Use `plan/QUICK_START.md` for short session-focused checklists.

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
  - **Note**: Physical GPIO deferred to Chapter 6 (WiFi-only async_blink for v0.1)
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

### T2.3 - Move Physical I/O Content to Chapter 6 (gpio-hardware.md)
- **Status**: ⏳ PENDING
- **Priority**: MEDIUM (defers hardware to v0.2, refocuses Chapter 3 on WiFi-only)
- **Effort**: 30 minutes
- **Description**:
  - Remove 'Adding Physical Hardware (Optional)' section from esp32-arduino.md ✓ Identified
    - GPIO Pin Assignment (GPIO 12 button, GPIO 13 LED) → Move to gpio-hardware.md
    - Bill of Materials (breadboard, button, LED, resistor, jumpers) → Move to gpio-hardware.md
  - Update gpio-hardware.md with moved content
  - Modify esp32-arduino.md 'What's Next' section to reference Chapter 6 for hardware integration
  - **Result**: Chapter 3 focused purely on WiFi async blink (no hardware setup needed)
- **Owner**: After T2.0.1 verified working
- **Depends On**: T2.0.1 (code working on hardware)
- **Blocks**: Chapter 6 completion

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

### T2.6.1 - Create OpenMRN-Lite Architecture Chapter (Chapter 3)
- **Status**: ✅ COMPLETED (2025-12-21)
- **Priority**: MEDIUM (informative, educational)
- **Effort**: 1.5 hours
- **Description**:
  - **Create new file**: `src/openmrn-architecture.md` (became Chapter 3) ✅
  - **Contents**:
    - Opening: "Why OpenMRN-Lite is the Right Choice for Learning OpenLCB"
    - The Alternative: ESP-IDF + full OpenMRN (threading complexity)
    - Why OpenMRN-Lite instead: lower learning curve, faster results, proven examples
    - What OpenMRN-Lite DOES Support: CDI, SNIP, events, datagrams, CAN, TCP
    - What OpenMRN-Lite Does NOT Support: virtual nodes, traction, bridging (why they need FreeRTOS)
    - Decision matrix: when to use each toolchain
    - Configuration & learning implications
  - **Add to SUMMARY.md** as Chapter 3 (between "Getting Started" and "ESP32 with Arduino") ✅
  - **Result**: Clear pedagogical rationale for library and platform choices ✅
- **Owner**: Completed 2025-12-21
- **Depends On**: T1.4 (Getting Started chapter) ✅
- **Blocks**: None

### T2.7 - Revise esp32-arduino.md for LccPro & Fix Config Comments
- **Status**: ✅ COMPLETED (2025-12-22)
- **Priority**: HIGH
- **Effort**: 2 hours
- **Description**:
  - ✅ Verify main.cpp works on hardware: builds, connects to WiFi, JMRI sees node and events
  - ✅ Revise "Testing with JMRI" section (around line 1000+):
    - ✅ Replace DecoderPro references with LccPro as authoritative tool
    - ✅ Add SNIP vs ACDI explanation (static device identity vs user-editable layout identity)
    - ✅ Note: SNIP appears in node list, ACDI User Name appears in Configure dialog
    - ✅ Defer node renaming demo to Chapter 5 Configuration & Persistence chapter
  - ✅ Fix config documentation in config.h and main.cpp:
    - ✅ Remove "read-only" language; clarify config IS saved to SPIFFS
    - ✅ Explain factory_reset() creates config file on first boot (initializes SNIP_NODE_NAME/DESC)
    - ✅ Clarify apply_configuration() is for applying runtime changes when config is modified via JMRI
    - ✅ Note: v0.1 has no config that impacts execution, so apply_configuration() returns UPDATED without changes
  - ✅ Add new subsection "Configuration & JMRI" explaining CDI role in configuration discovery
  - **Result**: Documentation matches working code; users understand config persistence and SNIP/ACDI distinction
- **Owner**: Completed 2025-12-22
- **Depends On**: None (main.cpp already complete from T2.0.1)
- **Blocks**: T5.1 (Configuration & Persistence chapter depends on understanding current state)

## Phase 3: Diagrams & Visualization

### T3.1 - Create Mermaid: Node Startup Sequence
- **Status**: ✅ Completed
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
- **Owner**: Completed 2025-12-24
- **Depends On**: None
- **Blocks**: None



---

## Phase 4: Configuration Chapter — Multi-Session Development

### T4.1 - Write Phase 1: Initial Configuration Content (Session 1)
- **Status**: ✅ COMPLETED (2025-12-24)
- **Priority**: HIGH
- **Effort**: 1.5-2 hours
- **Description**:
  - ✅ Created `src/04-configuration/` directory with 4 markdown files
  - ✅ Wrote overview.md: "Building on Chapter 3 concepts of CDI and SNIP"
  - ✅ **Sections Written**:
    1. overview.md: Why configuration matters, what's in this chapter, what's deferred
    2. storage-model.md: Offset-based storage, SNIP static vs. dynamic, CANONICAL_VERSION, CDI segments
    3. editing-in-jmri.md: Step-by-step LccPro workflow for renaming node (6 steps with placeholders)
    4. factory-reset.md: What happens on first boot, what gets preserved, serial console output examples
  - ✅ Updated SUMMARY.md to add Chapter 4 with initial sections
  - ✅ Updated forward references in Chapter 3 (code-configuration.md, code-walkthrough.md, jmri-monitoring.md)
  - ✅ Verified mdbook build with no errors
- **Testing**: Completed; mdbook builds successfully
- **Owner**: Completed 2025-12-24
- **Depends On**: None (builds on existing Chapter 3 knowledge)
- **Blocks**: T4.2, T4.3

### T4.2 - Research & Test Configuration Versioning (Session 2)
- **Status**: ✅ COMPLETED (2025-12-24)
- **Priority**: HIGH
- **Effort**: 1.5-2 hours
- **Description**:
  - ✅ **Research Completed** - Created comprehensive T4.2-RESEARCH.md document covering:
    - OpenLCB Standards: S-9.7.4.1/2/3 and corresponding TN documents
    - Configuration Update Workflow: Discovery → Retrieval → Modification → Activation
    - OpenMRNLite Implementation: Publish-subscribe pattern, ConfigUpdateFlow state machine
    - Standards Compliance Analysis: What's implemented, partial, and missing
    - Critical Gaps: No field migration, no persistent initialized flag, no dependency ordering
    - Memory Space Details: 0xFF (CDI), 0xFD (Config), 0xF0-F2 alternatives
  - ✅ **Key Findings**:
    - Version mismatch: Complete factory reset (all-or-nothing), not selective migration
    - What survives reset: Only first 128 bytes (ACDI user data: node name/description)
    - Offset 128+ completely wiped: factory_reset() callbacks apply defaults
    - No field mapping: Code does NOT migrate fields selectively
    - Reserved space workaround: Adding padding avoids version bumps during development
  - ✅ **Standards Verification**: Chapter 4 content is factually correct
- **Testing**: Research-only; no hardware testing needed
- **Owner**: Completed 2025-12-24
- **Depends On**: T4.1 ✅
- **Blocks**: T4.3

### T4.3 - Write Phase 3: Configuration Versioning Content & Code Implementation (Session 3)
- **Status**: � READY TO START
- **Priority**: HIGH
- **Effort**: 2-2.5 hours
- **Description**:
  - **Create new Chapter 4 section** (persistence-details.md was removed; content will be written during implementation based on practical EventInterval example):
    - Configurable Settings / Applying Configuration Changes: Practical implementation of apply_configuration()
    - Configuration Versioning: CANONICAL_VERSION role, what triggers resets
    - Reserved Space Technique: How to evolve schema without breaking configs
    - Version Mismatch Behavior: Factory reset triggers, SNIP preservation at offset 0-127
    - Hands-on Walkthrough: Modify EventInterval via JMRI, demonstrate runtime effect
    - Best Practices: When to bump version, when to use reserved space
  - **Code Implementation** in test/async_blink_esp32:
    - Add EventInterval uint16_t field to config.h CDI segment
    - Implement enhanced apply_configuration() to read EventInterval from config
    - Update loop() to use configurable interval instead of hardcoded EVENT_INTERVAL
    - Add logging to show when config changes are detected
    - Test with JMRI: change interval via Configure, verify firmware reads and applies it
  - **Update code comments** in config.h explaining reserved space pattern and field layout
- **Testing**: Requires ESP32 hardware, JMRI, and verifying EventInterval changes work
- **Owner**: Next session
- **Depends On**: T4.2 ✅
- **Blocks**: T4.4

### T4.4 - Integrate Chapter 4 into Book & Cross-References
- **Status**: ✅ COMPLETED (2025-12-25)
- **Priority**: HIGH
- **Effort**: 30-45 minutes
- **Description**:
  - Verify SUMMARY.md includes all Chapter 4 sections (already added in T4.1) ✅
  - Add cross-references after T4.3:
    - From Chapter 3 code-walkthrough.md: Link to Chapter 4 for "Configuration Versioning" deep dive
    - From Chapter 3 code-configuration.md: Link to Chapter 4 for "Configuration Persistence Patterns"
  - Update CHAPTERS.md in plan/ folder:
    - Add Chapter 4 full status and content summary
    - Note dependencies and future chapter relationships
  - Update plan/FORWARD_REFERENCES.md if any forward references to Chapter 5+ were added
- **Testing**: Run `mdbook build` and verify no errors, TOC includes all Chapter 4 sections
- **Owner**: After T4.3
- **Depends On**: T4.3 (content complete)
- **Blocks**: T5.1 (CAN transport chapter)
- **Completion Notes**: 
  - Updated SUMMARY.md to include all 7 Chapter 4 sections in correct order
  - Removed stub files (configuration-lifecycle.md, understanding-interval-setting.md) from SUMMARY to be populated later if needed
  - Updated "Next Steps" in adding-interval-setting.md to summarize Chapter 4 completion and preview CAN transport as next chapter
  - No placeholder TODOs remain in active Chapter 4 content files

---

## Phase 5: Future Chapters (CAN, GPIO, Advanced Topics)

### T5.1 - Switching to CAN Transport Chapter (FUTURE)
- **Status**: 📋 PLANNED
- **Priority**: MEDIUM (v0.3)
- **Effort**: 3-4 hours
- **Description**:
  - Create new file: `src/05-switching-to-can/switching-to-can.md` (becomes Chapter 5 in book)
  - Content:
    - Transport architecture comparison: WiFi/TCP vs CAN bus
    - Hardware requirements: CAN transceiver selection, wiring, termination
    - Porting async_blink_esp32 to use CAN instead of WiFi
    - Configuration changes needed (no more WiFi credentials, add CAN pin setup)
    - Testing with JMRI via CAN interface instead of TCP
  - Result: Users understand transport layer abstraction and can migrate to production CAN-based nodes
- **Owner**: Future session
- **Depends On**: T4.4 (Configuration chapter complete)
- **Blocks**: None

### T5.2 - Final Book Build & Verification
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

### T5.3 - Create Release Notes / Session Summary
- **Status**: ⏳ Not Started
- **Priority**: MEDIUM
- **Effort**: 30 minutes
- **Description**:
  - Update PROJECT_STATUS.md: mark v0.2 complete
  - Document what was accomplished:
    - Chapters revised/added (esp32-arduino.md for LccPro, new Configuration & Persistence chapter)
    - Examples enhanced (main.cpp config integration, EVENT_INTERVAL example)
    - Documentation clarifications (SNIP vs ACDI, config persistence, factory_reset vs apply_configuration)
  - List what's NOT included (CAN hardware, advanced features, GPIO integration) for future chapters
  - Note any open questions or TODOs for next iteration
- **Owner**: Final session
- **Depends On**: T5.2
- **Blocks**: None

---

## Work Prioritization Summary

**Session A (Next Implementation — T2.7):**
1. Verify main.cpp on hardware ✅
2. Revise esp32-arduino.md for LccPro (2 hours)
3. Fix config comments for clarity (30 min)
4. Update TASKS.md and documentation

**Session B (Following Implementation — T5.1):**
1. Enhance async_blink_esp32 with EVENT_INTERVAL config
2. Create Configuration & Persistence chapter (3-4 hours)
3. Document config versioning and schema evolution
4. Hands-on JMRI walkthrough

**If time is limited in future:**
1. **Must Have (Blocking)**: T1.1, T1.2, T2.1, T2.3, T2.4, T2.7, T5.1
2. **Should Have (High Value)**: T3.1, T3.2, T4.1
3. **Nice to Have (Polish)**: T3.3, T4.2, T5.2, T5.3
4. **Future**: CAN hardware, GPIO integration, advanced features

---

**Last Updated**: 2025-12-22  
**Next Review**: Before starting T2.7 implementation session
