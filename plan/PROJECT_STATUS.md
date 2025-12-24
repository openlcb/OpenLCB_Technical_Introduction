# Project Status - ESP32 WiFi Foundation (v0.1)

## Current Phase

**Phase**: Phase 2 - Example Code (T2.7 COMPLETED)  
**Milestone**: JMRI documentation updated for LccPro; Chapter 3 complete and ready for review  
**Status**: ✅ Phase 0 & Phase 1 Complete | ✅ Phase 2 Complete (T2.0-T2.7)  
**Last Updated**: 2025-12-22

## Strategic Decision: OpenMRN-Lite Architecture Clarification

**Decision Made**: 2025-12-21 (CORRECTED)  
**Previous assumption (incorrect)**: Full OpenMRN vs OpenMRN-Lite  
**Accurate architecture**:
- **OpenMRN-Lite IS the Arduino version** of OpenMRN (single-threaded, no RTOS required)
- **Full OpenMRN requires FreeRTOS/POSIX threading** and cannot run in Arduino environment
- **CDI is already supported in OpenMRN-Lite** - it was disabled in our async_blink example
- **No library migration needed** - enhance async_blink to use existing CDI support

**Correct Strategic Decision**:
- Keep OpenMRN-Lite (the only choice for Arduino/ESP32)
- Enable CDI in async_blink_esp32 (library supports it, we just need to configure it)
 - Hardcoded approach in v0.1 (same as before)
 - CDI-based configuration in Chapter 4 (planned)

**Impact on Planning**:
- T2.0.1 is now enhancement, not migration (no library changes needed)
- Chapter 3 uses OpenMRN-Lite with CDI (correct architecture from start)
- Chapter 3 (OpenMRN-Lite Architecture) explains the learning-first approach + what it supports
- Existing async_blink_esp32 code remains valid; just add CDI support

## Completed Items ✅

### Phase 0: Conceptual Restructuring
- [x] **T0.1**: Restructured intro.md for high-level overview
  - Updated "Focus of this book" to WiFi/TCP first approach
  - Added "Transport Layers" section (CAN vs WiFi/TCP rationale)
  - Rewrote intro_async_blink.md → "Node Startup Sequence" (concept-focused)
  - Added Mermaid sequence diagram for CID → RID → AMD → Init Complete flow
  - Rewrote intro_ab_events.md → "Events and Run Mode" 
  - Added Mermaid event flow diagram (button → LED interaction)
  - Added "Multi-Node Network Behavior" section
  - Updated SUMMARY.md chapter titles for clarity
  - Installed and configured mdbook-mermaid preprocessor
  - Updated FORWARD_REFERENCES.md tracking

### Phase 1: Content Restructuring & Preparation
- [x] **T1.1**: Archived Nucleo/STM32 content
  - Created src/archive/ directory
  - Moved openmrn.md → archive/openmrn-stm32.md
  - Removed from SUMMARY.md

- [x] **T1.2**: Created ESP32 chapter skeleton
  - New file: src/esp32-arduino.md with 8 section outlines
  - Added to SUMMARY.md as Chapter 3

- [x] **T1.3**: Updated book structure
  - SUMMARY.md reflects new chapter organization
  - Book builds successfully with mdbook
  - Mermaid diagrams render correctly

- [x] **T1.4**: Rewrote Getting Started chapter (NEW)
  - Archived old start.md → archive/start-nucleo.md
  - Complete rewrite focused on ESP32/WiFi approach
  - Added "Why WiFi/TCP First?" section (reinforces transport choice)
  - Added platform rationale, prerequisites, dev environment overview
  - Added high-level JMRI monitoring overview
  - Bridges conceptual Introduction with hands-on ESP32 chapter

### Infrastructure
- [x] Project vision and architecture decisions documented
- [x] Planning folder structure created (10 planning files)
- [x] Research completed on async_blink, OpenMRNLite, ESP32, standards
- [x] Task breakdown (24+ tasks across 5 phases)
- [x] Diagram requirements mapped (5 diagrams planned)

## In-Progress Items 🔄

None - Phase 1 complete, ready for Phase 2.

## Upcoming Priority Tasks

### Phase 2: Example Code & Hardware Documentation (IN PROGRESS) 🔄
- **T2.0** ✅ COMPLETED: Write PlatformIO Installation Guide (OpenMRNLite version)
- **T2.0.1** ✅ COMPLETED: Enhance async_blink_esp32 with CDI support
  - Keep OpenMRN-Lite library (no migration needed) ✅
  - Rewrite config.h to enable CDI structure ✅
  - Rewrite main.cpp to generate and expose CDI ✅
  - Test on ESP32 hardware, verify JMRI sees events and CDI ✅
- **T2.1** ✅ COMPLETED (OpenMRNLite) → ✅ UPDATED (via T2.0.1)
- **T2.2** ✅ COMPLETED (OpenMRNLite) → ✅ UPDATED (via T2.0.1)
- **T2.3** ✅ COMPLETED: Physical I/O Content deferred to Chapter 6
- **T2.4** ✅ COMPLETED: JMRI TCP setup guide (no changes needed)
- **T2.5** 📋 DEFERRED: Make WiFi Configuration & Event IDs Configurable (Chapter 4)
- **T2.6** ✅ COMPLETED (2025-12-21): Updated Chapter 3 Documentation for OpenMRN-Lite CDI
  - Updated config.h code block with SNIP_NODE_NAME, SNIP_NODE_DESC, Acdi, UserInfoSegment ✅
  - Updated main.cpp with refactored helper functions ✅
  - Enhanced code walkthrough with SPIFFS/SNIP explanations ✅
  - Added 9 detailed code walkthrough sections ✅
- **T2.6.1** ✅ COMPLETED (2025-12-21): Create Chapter 3 (OpenMRN-Lite Architecture)
- **T2.7** ✅ COMPLETED (2025-12-22): Revise esp32-arduino.md for LccPro & Fix Config Comments
  - Verified main.cpp works on ESP32 hardware (builds, WiFi connects, JMRI sees node/events) ✅
  - Replaced DecoderPro references with LccPro (authoritative tool in JMRI 5.12+) ✅
  - Added LccPro Wizard screenshot (LccPro_FirstConnection.png) for first-time setup ✅
  - Added JMRI Preferences screenshot (JMRI_Preferences.png) for existing connections ✅
  - Added LccProNodes.png showing node list with SNIP data ✅
  - Added SNIP vs ACDI explanation (device identity vs layout identity) ✅
  - Fixed config documentation comments (clarified SPIFFS persistence, factory_reset, apply_configuration) ✅
  - Added "Configuration & JMRI: CDI Discovery" subsection explaining config discovery role ✅
  - Updated menu references to use "ESP32 LCC" (connection name) ✅
  - Updated JMRI minimum version requirement to 5.12 ✅
  - **Result**: Chapter 3 documentation complete and consistent with working code

### Phase 3: Diagrams & Visualization
- Mermaid diagrams already complete in Phase 0! ✅
  - Node Startup Sequence diagram (in intro_async_blink.md)
  - Event Flow diagram (in intro_ab_events.md)
- **T3.3**: (Optional) Create breadboard schematic diagram

### Phase 4: Integration & Testing
- **T4.1**: End-to-end testing & verification guide
- **T4.2**: Create "What's Next" preview section

### Phase 5: Book Build & Release
- **T5.1**: Final book build & verification
- **T5.2**: Create release notes / session summary

## Known Blockers 🚧

None currently blocking progress.

## Key Unknowns (TBD)

| Item | Impact | Status |
|------|--------|--------|
| Exact GPIO pins for button/LED | Medium | Will decide during example creation |
| Event ID scheme | Low | Use fixed IDs for v1, defer custom generation |
| WiFi credential approach | Low | Hardcode for v1, defer SPIFFS config |
| Breadboard schematic format | Low | Mermaid or PNG export; decide when creating |

## Summary of Key Findings

### async_blink Example
- Located in `openmrn/applications/async_blink/`
- Builds for 26+ platforms (Linux, macOS, STM32, ESP8266, etc.)
- Uses recursive Makefile system
- Alternates between two event IDs every 1 second
- Can run without CAN hardware on desktop targets

### ESP32 Arduino Support
- **Full support** via OpenMRNLite library (Arduino package)
- Supports **WiFi** and **CAN transceiver** modes
- Requires arduino-esp32 core v1.0.1+
- CAN requires external MCP2551 or SN65HVD230 transceiver
- **v1 approach**: Use WiFi/TCP, skip CAN hardware

### Documentation Gaps
- No async_blink build instructions in openmrn.md
- No Arduino/ESP32 setup coverage
- No PlatformIO guidance
- No breadboard hardware wiring details
- No JMRI TCP configuration for ESP32

## Risk Assessment

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|-----------|
| PlatformIO build complexity | Low | Medium | Create tested platformio.ini template |
| WiFi/TCP stability issues | Low | Medium | Test early; have fallback to localhost testing |
| GPIO conflicts with OpenMRNLite | Low | Low | Check library pin defaults during example creation |
| JMRI connection problems | Low | Medium | Document TCP connection steps with screenshots |

---

**Last Updated**: 2025-12-20  
Next Session: Start Phase 3 (diagrams and testing/verification)
