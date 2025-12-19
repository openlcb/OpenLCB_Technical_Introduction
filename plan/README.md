# LCC Technical Introduction Book - Planning Guide

## Project Vision

Create an online book that helps embedded C++ developers understand and implement OpenLCB (Low-Cost Control Bus) on microcontrollers, starting with practical, hands-on examples on ESP32 running Arduino/PlatformIO, progressing to advanced topics like CAN hardware transport.

## Target Audience

- Embedded C++ developers with microcontroller experience
- People wanting to build their own LCC boards/nodes
- Developers using the OpenMRN library

## v1 Release Focus: ESP32 WiFi Foundation

**Goal**: Get an async_blink_esp32 example running on ESP32 via PlatformIO with WiFi/TCP transport, controlled via breadboard input (button) and output (LED), monitoring via JMRI.

**Why this approach?**
- WiFi/TCP is simpler than CAN hardware for initial learning
- Provides tangible hardware interaction (button press → event → LED toggle)
- Establishes foundation for adding CAN transport in future chapters
- Readers can get working code in first session without special hardware

## Key Architecture Decisions

| Decision | Rationale |
|----------|-----------|
| **PlatformIO + Arduino IDE** | Modern toolchain, easy build configuration, good platform support |
| **WiFi/TCP transport (v1)** | No CAN hardware needed initially; switch to CAN in later chapters |
| **Breadboard circuit (button + LED)** | Makes events tangible; demonstrates real input/output device pattern |
| **Fixed event IDs (v1)** | Simplify learning curve; show custom event generation in later chapter |
| **JMRI for monitoring** | Existing tool; allows verification without custom tooling |
| **Mermaid diagrams** | VS Code + mdBook support; version-controllable; easy to maintain |
| **Archive Nucleo/STM32 content** | Reduce v1 scope; preserve for reference appendix later |

## File Organization

```
src/
├── intro.md                          # Introduction + startup theory (RESTRUCTURE)
├── start.md                          # Getting started (update)
├── openmrn.md                        # ARCHIVE THIS for later
├── plan/
│   ├── README.md                     # This file - project vision
│   ├── 00_OVERVIEW.md                # Executive summary
│   ├── QUICK_START.md                # Next session workflow + templates
│   ├── PROJECT_STATUS.md             # Current phase, blockers, completed
│   ├── TASKS.md                      # Prioritized work items
│   ├── CHAPTERS.md                   # Book outline + content status (UPDATED)
│   ├── RESEARCH.md                   # Research findings + references
│   ├── DIAGRAMS.md                   # Diagram inventory
│   ├── SUB_AGENT_GUIDE.md            # When/how to use sub-agents
│   ├── STYLE_GUIDE.md                # Writing style for consistency
│   └── FORWARD_REFERENCES.md         # ✨ Track forward references to future chapters
└── archive/                          # (TBD) Nucleo/STM32 content
```
└── archive/                          # (TBD) Nucleo/STM32 content
````
    └── openmrn-stm32.md              # Move here
```

## Session Workflow

### At Session Start

1. **Read PROJECT_STATUS.md** - Understand current phase and blockers
2. **Review TASKS.md** - Pick next priority item to work on
3. **Check DIAGRAMS.md** - See which diagrams block content completion
4. **Skim RESEARCH.md** - Refresh key findings about async_blink, ESP32, OpenMRN

### During Session

- Update TASKS.md as work progresses (mark items in-progress, completed, blocked)
- Add findings to RESEARCH.md if new discoveries made
- Update CHAPTERS.md if content structure changes
- Note any new diagram needs in DIAGRAMS.md

### At Session End

- Update PROJECT_STATUS.md with accomplishments and blockers
- Summarize work in TASKS.md
- Leave TODO items clearly marked for next session

## Key Files Outside Plan Folder

| File | Purpose | Status |
|------|---------|--------|
| book.toml | mdBook config | ✓ Exists |
| src/intro.md | LCC intro + startup theory | 🔄 Needs Mermaid diagrams |
| src/start.md | Getting started overview | 🔄 Needs update |
| openmrn/applications/async_blink/ | Desktop example (reference) | ✓ Exists |
| openmrn/arduino/OpenMRNLite/ | ESP32 Arduino library | ✓ Exists |
| markdown/standards/ | Technical references | ✓ Available |

## Important Constraints & Notes

- **No Windows build guidance** for async_blink - stick to PlatformIO/Arduino for simplicity
- **TCP before CAN** - Intentional sequencing to reduce initial setup complexity
- **JMRI monitoring required** - Users need a way to observe startup messages and events
- **Breadboard realistic** - Real-world I/O pattern; prepares for actual use cases

---

**Last Updated**: 2025-12-18  
**Session**: Initial planning setup  
**Next Session**: Begin with TASKS.md items
