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

## Phase 4: Configuration Chapter — Multi-Session Development ✅ COMPLETE

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
- **Status**: ✅ COMPLETED (2025-12-25)
- **Priority**: HIGH
- **Effort**: 2-2.5 hours
- **Description**:
  - **Created new Chapter 4 section** (adding-interval-setting.md): ✅
    - Configurable Settings: Practical implementation of apply_configuration() ✅
    - Configuration Versioning: CANONICAL_VERSION role explained ✅
    - Hands-on Walkthrough: Step-by-step JMRI configuration guide ✅
    - Best Practices: When to use configurable vs hardcoded values ✅
  - **Code Implementation** in test/async_blink_esp32: ✅
    - Added blink_interval Uint16ConfigEntry field to config.h CDI segment ✅
    - Implemented apply_configuration() to read blink_interval from config ✅
    - Updated loop() to use configurable interval (0 = disabled) ✅
    - Added logging to show when config changes are detected ✅
    - Tested with JMRI: interval changes work, configuration persists ✅
  - **Updated code comments** in config.h explaining configuration structure ✅
- **Testing**: Completed on ESP32 hardware with JMRI ✅
- **Owner**: Completed 2025-12-25
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

## Phase 5: CAN Transport Chapter ✅ COMPLETE

### T5.1 - Switching to CAN Transport Chapter
- **Status**: ✅ COMPLETED (2025-12-29)
- **Priority**: HIGH
- **Effort**: 3-4 hours
- **Description**:
  - Created Chapter 5: `src/05-can/` with complete CAN transport documentation ✅
  - Content completed:
    - Understanding CAN Transceivers (01-can-transceivers.md) ✅
    - Hardware Wiring: Breadboard Setup (02-hardware-wiring.md) ✅
    - Code Changes: From WiFi to CAN (03-code-changes.md) ✅
    - The Esp32HardwareTwai Class (04-esp32-class-naming.md) ✅
    - Configuring JMRI for CAN (05-jmri-configuration.md) ✅
    - TWAI Bus Diagnostics and Troubleshooting (06-twai-diagnostics.md) ✅
  - Updated async_blink_esp32 code to support CAN transport ✅
  - Tested CAN transport with JMRI ✅
- **Owner**: Completed 2025-12-29
- **Depends On**: T4.4 (Configuration chapter complete) ✅
- **Blocks**: None

---

## Phase 6: GPIO/Physical I/O Implementation

### T6.1 - Add Reserved Configuration Space for Future Expansion
- **Status**: ✅ COMPLETED (2025-12-30)
- **Priority**: HIGH
- **Effort**: 1 hour
- **Description**:
  - Modified `test/async_blink_esp32/include/config.h`: ✅
    - Added `blink_enabled` (Uint8ConfigEntry with Enabled/Disabled MapValues) ✅
    - Added `reserved` (BytesConfigEntry<32>) after blink_interval ✅
    - Incremented CANONICAL_VERSION to 0x0003 (triggers one-time factory reset) ✅
  - Modified `test/async_blink_esp32/src/main.cpp`: ✅
    - Added global `bool blink_enabled` variable ✅
    - Updated `apply_configuration()` to read both blink_enabled and blink_interval ✅
    - Updated `factory_reset()` to initialize blink_enabled to 1 (Enabled) ✅
    - Updated `loop()` to check `if (blink_enabled && ...)` before producing events ✅
  - Tested configuration persistence: ✅
    - Verified factory reset on first boot with new version ✅
    - Tested enable/disable feature via JMRI (dropdown shows Enabled/Disabled) ✅
    - Verified configuration persists across power cycles ✅
  - Created `src/06-gpio/01-enable-disable.md`: ✅
    - Explained reserved space pattern for configuration evolution ✅
    - Documented how future fields will consume reserved bytes without version increment ✅
    - Covered CDI caching behavior (must restart JMRI/LccPro after CDI changes) ✅
    - Included complete code diffs using Highlight.js format ✅
  - Updated `src/06-gpio/overview.md`: ✅
    - Restructured as chapter introduction (two-part approach) ✅
    - Updated GPIO pin assignments to avoid CAN conflict (GPIO 18/19/21/22) ✅
    - Updated hardware BOM for 2 buttons and 2 LEDs ✅
  - Updated `src/SUMMARY.md`: ✅
    - Added Chapter 6 with 01-enable-disable.md section ✅
- **Owner**: Completed 2025-12-30
- **Depends On**: None (builds on completed Chapter 5) ✅
- **Blocks**: T6.2

### T6.2 - Add Single Button Producer with ConfiguredProducer
- **Status**: ✅ COMPLETED (2025-12-30)
- **Priority**: HIGH
- **Effort**: 2-3 hours
- **Description**:
  - ✅ Modified `test/async_blink_esp32/include/config.h`:
    - Incremented CANONICAL_VERSION to 0x0004 (button configuration added)
    - Added `CDI_GROUP_ENTRY(button, ProducerConfig, Name("Button 1"))` 
    - No reserved space consumed yet (saved for future expansion)
  - ✅ Modified `test/async_blink_esp32/src/main.cpp`:
    - Added GPIO pin definition: `GPIO_PIN(BUTTON, GpioInputPU, 18)` (active-low with pull-up)
    - Added GpioInitializer: `typedef GpioInitializer<BUTTON_Pin> GpioInit`
    - Created ConfiguredProducer: `openlcb::ConfiguredProducer button_producer(...)`
    - Created RefreshLoop for 33Hz polling: `openlcb::RefreshLoop button_refresh_loop(...)`
    - Called `GpioInit::hw_init()` before `openmrn.begin()` in setup()
  - ✅ Updated FactoryResetHelper::factory_reset():
    - Button event_on = NODE_ID + 0x0100 (released/HIGH)
    - Button event_off = NODE_ID + 0x0101 (pressed/LOW)
    - Default description: "Button 1"
    - Default debounce: 3 (90ms at 33Hz)
  - ✅ Tested on hardware:
    - Button wired to GPIO 18 (one side) and GND (other side)
    - LccPro Traffic Monitor shows events when pressed/released
    - Configuration dialog shows button settings (description, debounce, event IDs)
    - Active-low behavior verified (pressed = LOW/Off, released = HIGH/On)
  - ✅ Created `src/06-gpio/02-button-input.md`:
    - Hardware setup with breadboard wiring diagram
    - Complete code walkthrough (GPIO definition, ProducerConfig, RefreshLoop, factory reset)
    - Event ID allocation strategy (0x0100 offset for visual clarity)
    - Active-low vs active-high explanation with OpenMRN pull-down bug documentation
    - Testing workflow (Traffic Monitor first, then configuration viewing after restart)
    - Troubleshooting section
  - ✅ Updated `src/SUMMARY.md` with new section
- **Owner**: Completed 2025-12-30
- **Depends On**: T6.1 ✅
- **Blocks**: T6.3

### T6.3 - Add Single LED Consumer with ConfiguredConsumer
- **Status**: ✅ Completed
- **Priority**: HIGH
- **Effort**: 2 hours
- **Description**:
  - ✅ Modified `test/async_blink_esp32/include/config.h`:
    - Added `#include "openlcb/ConfiguredConsumer.hxx"`
    - Added `CDI_GROUP_ENTRY(led, ConsumerConfig, Name("LED 1"), Description("LED output on GPIO 19"))`
    - Incremented CANONICAL_VERSION to 0x0005
  - ✅ Modified `test/async_blink_esp32/src/main.cpp`:
    - Added GPIO pin definition: `GPIO_PIN(LED, GpioOutputSafeLow, 19);`
    - Updated GpioInitializer: `typedef GpioInitializer<BUTTON_Pin, LED_Pin> GpioInit;`
    - Created ConfiguredConsumer instance: `openlcb::ConfiguredConsumer led_consumer(openmrn.stack()->node(), cfg.seg().led(), LED_Pin());`
  - ✅ Updated FactoryResetHelper::factory_reset():
    - Wrote independent event IDs for LED: event_on = NODE_ID + 0x0200, event_off = NODE_ID + 0x0201
    - Wrote default description: "LED 1"
    - Added serial output for LED event IDs
  - ✅ Tested on hardware:
    - Built and uploaded successfully
    - Verified LED responds to manual events via LccPro Send Frame
    - Successfully configured LED to respond to button events via Configure dialog
    - Verified button→LED workflow (press button = LED on)
    - Configuration persists across power cycles
  - ✅ Wrote `src/06-gpio/03-led-output.md`:
    - Breadboard wiring diagram with resistor selection guide
    - Complete code walkthrough with diffs
    - Two testing methods:
      - Manual testing with LccPro Send Frame
      - Configuration workflow with copy/paste event IDs
    - ConfiguredConsumer explanation with openmrn.loop() clarification
    - Active-high LED vs active-low button logic explanation
    - Experimenting section (Factory Reset, trying blink events)
    - Comprehensive troubleshooting section
  - ✅ Updated `src/SUMMARY.md` with new section
- **Owner**: Completed 2025-12-30
- **Depends On**: T6.2 ✅
- **Blocks**: T6.4

### T6.4 - Scale to Multiple I/O with Multi Patterns
- **Status**: ⏳ Not Started
- **Priority**: HIGH
- **Effort**: 3-4 hours
- **Description**:
  - Research Multi patterns:
    - Confirm MultiConfiguredConsumer exists and usage pattern
    - Research whether MultiConfiguredProducer exists in OpenMRNLite
    - If MultiConfiguredProducer doesn't exist, plan to use individual ConfiguredProducer instances
    - Document findings
  - Modify `test/async_blink_esp32/include/config.h`:
    - Consume remaining reserved space (8 bytes → 0 bytes)
    - Replace single entries with RepeatedGroup:
      - `constexpr uint8_t NUM_BUTTONS = 2;`
      - `constexpr uint8_t NUM_LEDS = 3;`
      - `using AllProducers = RepeatedGroup<ProducerConfig, NUM_BUTTONS>;`
      - `using AllConsumers = RepeatedGroup<ConsumerConfig, NUM_LEDS>;`
      - `CDI_GROUP_ENTRY(buttons, AllProducers, Name("Buttons"), RepName("Button"));`
      - `CDI_GROUP_ENTRY(leds, AllConsumers, Name("LEDs"), RepName("LED"));`
    - Remove old single button/led entries
    - **DO NOT increment CANONICAL_VERSION** - total config size unchanged (32 bytes reserved → 2 buttons + 3 LEDs consumed)
  - Modify `test/async_blink_esp32/src/main.cpp`:
    - Add new GPIO pins:
      - `GPIO_PIN(Button2, GpioInputPU, 21);` (second button)
      - `GPIO_PIN(LED2, GpioOutputSafeLow, 22);`
      - `GPIO_PIN(LED3, GpioOutputSafeLow, 23);`
    - Update GpioInitializer: `typedef GpioInitializer<Button1_Pin, Button2_Pin, LED1_Pin, LED2_Pin, LED3_Pin> GpioInit;`
    - For producers: Create individual instances or use Multi pattern if available
      - `openlcb::ConfiguredProducer button1_producer(node, cfg.seg().buttons().entry<0>(), Button1_Pin());`
      - `openlcb::ConfiguredProducer button2_producer(node, cfg.seg().buttons().entry<1>(), Button2_Pin());`
      - Update RefreshLoop: `{ button1_producer.polling(), button2_producer.polling() }`
    - For consumers: Use MultiConfiguredConsumer pattern
      - `constexpr const Gpio *const led_pins[] = { LED1_Pin::instance(), LED2_Pin::instance(), LED3_Pin::instance() };`
      - `openlcb::MultiConfiguredConsumer led_consumers(node, led_pins, ARRAYSIZE(led_pins), cfg.seg().leds());`
  - Update FactoryResetHelper::factory_reset():
    - Use factory_reset_names() for repeated groups:
      - `openlcb::factory_reset_names(fd, cfg.seg().buttons(), "Button");` (creates "Button 1", "Button 2")
      - `openlcb::factory_reset_names(fd, cfg.seg().leds(), "LED");` (creates "LED 1", "LED 2", "LED 3")
    - Write independent event IDs for all buttons and LEDs (no cross-wiring in factory defaults)
  - Test on hardware:
    - Build and upload
    - Verify all buttons produce events independently
    - Verify all LEDs respond to events independently
    - Use JMRI to create cross-wiring scenarios:
      - Wire Button 1 → LED 2
      - Wire Button 2 → LED 1 + LED 3 (same event ID to multiple consumers)
    - Verify JMRI shows repeated groups with RepName labels ("Button 1", "Button 2", etc.)
    - Confirm configuration persists and no factory reset occurred
  - Write `src/06-gpio/scaling-multiple-io.md`:
    - Explain RepeatedGroup<T, N> pattern for scalable configuration
    - Document MultiConfiguredConsumer memory efficiency advantages:
      - Single event handler for all LEDs vs N separate handlers
      - Show memory savings calculation for 3 LEDs
      - Explain user_arg encoding for pin indexing
    - Show factory_reset_names() helper usage
    - Demonstrate cross-wiring examples via JMRI
    - Explain why producers use individual instances (or Multi if researched and available)
    - Complete reserved space evolution walkthrough showing 32→20→8→0 byte progression
- **Owner**: Next session
- **Depends On**: T6.3
- **Blocks**: T6.5

### T6.5 - Finalize Chapter 6 and Evaluate Chapter 7 Scope
- **Status**: ⏳ Not Started
- **Priority**: HIGH
- **Effort**: 2 hours
- **Description**:
  - Review and update `src/06-gpio/overview.md`:
    - Update with tested GPIO assignments (18, 19, 21, 22, 23)
    - Add ESP32 pin safety reference table:
      - Safe pins for general use (18, 19, 21, 22, 23, 32, 33)
      - Bootstrap pins with warnings (0, 2, 5, 12, 15)
      - Input-only pins (34-39)
      - Reserved pins to avoid (6-11 flash, 1/3 UART)
    - Explain GPIO 20 reserved for future servo (PWM capable)
  - Create Mermaid diagrams:
    - Sequence diagram: Button press → Producer → Event transmission → Consumer → LED response
    - Timing diagram: RefreshLoop polling cycle showing 33Hz frequency and debounce behavior
    - State diagram: ConfigUpdateFlow applying configuration to producers/consumers
  - Update `src/SUMMARY.md`:
    - Add Chapter 6 subchapters:
      - Configuration Evolution (01-enable-disable.md)
      - Single Button and LED (single-button-led.md)
      - Scaling to Multiple I/O (scaling-multiple-io.md)
      - (Placeholder for servo if Chapter 6, or forward ref if Chapter 7)
  - Evaluate chapter length and complexity:
    - Count total pages/sections in Chapter 6
    - Assess if servo should be:
      - Option A: Chapter 6 final section (if chapter is <10 pages)
      - Option B: New Chapter 7 "Advanced I/O: Servos and PWM" (if chapter is >10 pages)
      - Option C: Chapter 7 "Scaling I/O" with both servo and additional digital I/O patterns
  - Update planning files:
    - `plan/PROJECT_STATUS.md`: Mark Phase 6 complete, document servo decision
    - `plan/FORWARD_REFERENCES.md`: Add forward reference if servo becomes Chapter 7
    - `plan/TASKS.md`: Add Phase 7 tasks if servo chapter is split out
  - Document decision rationale:
    - Chapter length considerations
    - Pedagogical flow (simple→complex)
    - Reader feedback if available
- **Owner**: Next session
- **Depends On**: T6.4
- **Blocks**: Phase 7 (if servo becomes separate chapter)

---

**Last Updated**: 2025-12-29  
**Next Review**: Before starting T6.1 implementation session
