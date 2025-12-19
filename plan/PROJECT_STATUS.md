# Project Status - ESP32 WiFi Foundation (v0.1)

## Current Phase

**Phase**: Planning & Setup  
**Milestone**: Create planning infrastructure and research summary  
**Status**: 🔄 In Progress  

## Completed Items ✅

- [x] Project vision and architecture decisions documented
- [x] Planning folder structure created
- [x] Research completed on:
  - async_blink location and build system
  - Arduino/ESP32 OpenMRNLite library capabilities
  - Documentation gaps in openmrn.md
  - Diagram opportunities

## In-Progress Items 🔄

- [ ] Plan folder documentation (README, TASKS, CHAPTERS, RESEARCH, DIAGRAMS, SUB_AGENT_GUIDE)

## Upcoming Priority Tasks

### Phase 0: Conceptual Restructuring (NEW - HIGHEST PRIORITY)
- **T0.1**: Restructure intro.md for high-level overview (instead of jumping into implementation details)
  - Transport layers (CAN vs WiFi)
  - Startup sequence overview with multi-node participation
  - Forward references to future chapters
  - Add diagrams to clarify concepts

### Phase 1: Content Restructuring (After Phase 0)
- Archive Nucleo/STM32 content (move openmrn.md to archive/)
- Create chapter skeleton for "ESP32 with Arduino & PlatformIO"
- Expand restructured intro.md with Mermaid diagrams
- Add JMRI + TCP integration guide to start.md

### Phase 2: Example & Documentation
- Create async_blink_esp32 Arduino sketch with breadboard I/O
- Create platformio.ini template for ESP32 + OpenMRNLite
- Document breadboard circuit (button + LED wiring)
- Add GPIO pin assignment guide

### Phase 3: Diagrams & Visualization
- Create Mermaid Node Startup Sequence diagram
- Create Mermaid Event State Machine diagram
- (Optional) Create breadboard schematic diagram

### Phase 4: Integration & Testing
- Verify async_blink_esp32 builds and runs on ESP32
- Test JMRI connection via TCP
- Document verification steps and expected output
- Create troubleshooting guide for common issues

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

**Last Updated**: 2025-12-18  
**Next Session**: Focus on Phase 1 tasks (restructuring content)
