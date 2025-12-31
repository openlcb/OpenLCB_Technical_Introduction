# Project Status - ESP32 WiFi Foundation (v0.1)

## Current Phase

**Phase**: Phase 6 - GPIO/Physical I/O Implementation  
**Milestone**: Chapter 6 with button producers, LED consumers, and Multi I/O patterns; demonstrate configuration evolution with reserved space  
**Status**: ✅ Phase 0-2 Complete | ✅ Phase 3 Complete (Diagrams) | ⏳ Phase 4-5 Deferred | 🔄 Phase 6 Starting  
**Last Updated**: 2025-12-29

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

### Phase 4: Configuration & Persistence
- [x] **Chapter 4 Complete**: Configuration & Persistence chapter written and integrated
  - Storage model documentation
  - JMRI editing workflow
  - Factory reset behavior
  - Configurable blink interval example (added to async_blink_esp32)

## In-Progress Items 🔄

**Phase 6: GPIO/Physical I/O** - Starting implementation (see Upcoming Priority Tasks below)

## Upcoming Priority Tasks

### Phase 6: GPIO/Physical I/O Implementation (IN PROGRESS) 🔄
- **T6.1** ✅ COMPLETED: Add Reserved Configuration Space
  - ✅ Added blink_enabled and blink_interval configuration
  - ✅ Incremented CANONICAL_VERSION to 0x0003
  - ✅ Wrote 01-enable-disable.md explaining binary config pattern
  - ✅ Tested configuration persistence
- **T6.2** ✅ COMPLETED: Add Single Button Producer
  - ✅ Added ConfiguredProducer with GPIO 18 button
  - ✅ Added RefreshLoop for 33Hz polling
  - ✅ Incremented CANONICAL_VERSION to 0x0004
  - ✅ Tested button events in LccPro
  - ✅ Wrote 02-button-input.md documentation
- **T6.3** ✅ COMPLETED: Add Single LED Consumer
  - ✅ Added ConfiguredConsumer with GPIO 19 LED
  - ✅ Incremented CANONICAL_VERSION to 0x0005
  - ✅ Tested LccPro Send Frame and Configure workflows
  - ✅ Wrote 03-led-output.md documentation
- **T6.4** ⏳ NOT STARTED: Scale to Multiple I/O
  - Research MultiConfiguredProducer/Consumer patterns ⏳
  - Add RepeatedGroup<ProducerConfig, 2> and RepeatedGroup<ConsumerConfig, 3> ⏳
  - Add GPIO 21 (button 2), 22 (LED 2), 23 (LED 3) ⏳
  - Consume remaining 8 bytes of reserved space ⏳
  - Test cross-wiring scenarios via JMRI ⏳
  - Write scaling-multiple-io.md documentation ⏳
- **T6.5** ⏳ NOT STARTED: Finalize Chapter 6 and Evaluate Chapter 7
  - Update overview.md with GPIO pin safety table ⏳
  - Create Mermaid diagrams (sequence, timing, state) ⏳
  - Update SUMMARY.md with Chapter 6 subchapters ⏳
  - Decide if servo becomes Chapter 6 section or Chapter 7 ⏳

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

### Phase 6: GPIO Implementation Strategy

**Configuration Evolution Pattern**:
- Reserved space (BytesConfigEntry<32>) enables adding fields without version changes
- CANONICAL_VERSION remains unchanged as reserved bytes are consumed
- Progression: 32 bytes reserved → 20 (button) → 8 (LED) → 0 (multi I/O)
- Prevents factory reset and preserves user configuration during firmware updates

**GPIO Patterns**:
- **ConfiguredProducer**: Polling-based button input, requires RefreshLoop at 33Hz
- **ConfiguredConsumer**: Event-driven LED output, no polling needed
- **MultiConfiguredConsumer**: Memory-efficient pattern for multiple outputs (single event handler)
- **RepeatedGroup<T, N>**: CDI pattern for scalable I/O configuration

**Pin Assignments** (ESP32 DevKit):
- GPIO 18: Button 1 (GpioInputPU, active-low with pullup)
- GPIO 19: LED 1 (GpioOutputSafeLow)
- GPIO 21: Button 2 (GpioInputPU)
- GPIO 22: LED 2 (GpioOutputSafeLow)
- GPIO 23: LED 3 (GpioOutputSafeLow)
- GPIO 20: Reserved for future servo (PWM capable)

**Event ID Strategy**:
- factory_reset() writes independent event IDs for all producers/consumers
- No automatic cross-wiring in defaults
- Users configure event wiring via JMRI (demonstrates configuration workflow)

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

**Last Updated**: 2025-12-29  
Next Session: Start Phase 6 with T6.1 (Reserved Configuration Space)
