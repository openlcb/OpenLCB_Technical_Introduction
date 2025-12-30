# Quick Start: Next Session Workflow

> NOTE: This file is the immediate session playbook — a short, actionable subset of the canonical backlog in `plan/TASKS.md`.

**Last Updated**: 2025-12-30  
**Current Phase**: Phase 6 In Progress - GPIO/Physical I/O Implementation

## For Next Session: Start Here

### ⚠️ IMMEDIATE TASK: T6.3 (Add Single LED Consumer with ConfiguredConsumer)
**This session's focus**: LED output with ConfiguredConsumer pattern

1. **Read These Files First** (10 minutes):
   - **plan/TASKS.md** - T6.3 plan
   - **OpenMRNLite/src/openlcb/ConfiguredConsumer.hxx** - ConfiguredConsumer implementation
   - **src/06-gpio/02-button-input.md** - Just completed; understand button producer pattern

2. **Execute T6.3** (2 hours): Add single LED with GPIO consumer
   - Modify test/async_blink_esp32/include/config.h: Consume reserved space (20→8 bytes), add ConsumerConfig
   - Modify test/async_blink_esp32/src/main.cpp: Add GPIO pin, ConfiguredConsumer
   - Test on hardware: Wire LED to GPIO 19, configure via JMRI to respond to button events
   - **DO NOT increment CANONICAL_VERSION** (consuming reserved space, file size unchanged)

3. **After T6.3 Works**: Move to T6.4
   - Scale to multiple I/O with Multi patterns
   - Demonstrate RepeatedGroup configuration## Previous Sessions (Completed)

- ✅ T0.1: Restructure intro.md for high-level overview
- ✅ T1.1-T1.4: Initial content restructuring (archive STM32, create ESP32 chapter skeleton)
- ✅ T2.0-T2.0.1: OpenMRN-Lite CDI enhancement (working code on hardware)
- ✅ T2.1-T2.2: Create async_blink_esp32 Arduino Sketch and PlatformIO configuration
- ✅ T2.4: Create detailed JMRI TCP setup guide
- ✅ T2.6-T2.7: Update Chapter 3 documentation and create OpenMRN-Lite architecture chapter
- ✅ T3.1: Create Mermaid startup sequence diagram
- ✅ T4.1-T4.4: Complete Chapter 4 Configuration & Persistence (all sections, tested on hardware)
- ✅ T5.1: Complete Chapter 5 Switching to CAN Transport (all sections, tested with JMRI)
- ✅ T6.1: Add reserved configuration space, enable/disable feature, Chapter 6 Part 1 documentation
- ✅ T6.2: Add single button producer with ConfiguredProducer (GPIO 18, tested with LccPro)
## Key Planning Files

- **plan/TASKS.md** - Canonical backlog (T6.2 is next)
- **plan/PROJECT_STATUS.md** - Current phase status and blockers
- **plan/CHAPTERS.md** - Book structure and content status
- **plan/RESEARCH.md** - Technical findings and discoveries
- **plan/DIAGRAMS.md** - Diagram inventory and status
- **plan/STYLE_GUIDE.md** - Writing style & tone
- **plan/SUB_AGENT_GUIDE.md** - When to use sub-agents for research
- **OpenMRNLite/README.md** - Official library documentation
- **markdown/standards/S-9.7.4.*.md** - OpenLCB standards for reference

### 4. During Work
- Update TASKS.md as you progress (mark in-progress, completed)
- Add discoveries to RESEARCH.md if new info found
- Update DIAGRAMS.md if diagram status changes

### 5. End of Session
- Update PROJECT_STATUS.md with accomplishments and any new blockers
- Note what's ready for next session in TASKS.md

---

## Session Template (Copy & Use)

```markdown
# Session: [Date] - [Focus Area]

## Goals This Session
- [ ] Task T1.X: [description]
- [ ] Task T2.X: [description]
- [ ] Task T3.X: [description]

## Work Log

### Started T1.X at [time]
- [What I did]
- [Decisions made]
- [Blockers if any]
**Status**: [Completed | In Progress | Blocked]

### Started T2.X at [time]
- [What I did]
...

## Summary
- **Completed**: T1.X ✅, T2.X ✅
- **In Progress**: T3.X (XX% done)
- **Blockers**: None | [List any]
- **Next Session**: Should focus on T4.X

## Files Modified
- src/esp32-arduino.md (created)
- book.toml (updated TOC)
- plan/TASKS.md (updated status)
```

---

## Common Workflows

### Workflow A: Content Writing Session

1. Pick content task (T1.2, T2.4, etc.)
2. **Read plan/STYLE_GUIDE.md first** - understand tone, structure, terminology
3. Read CHAPTERS.md for outline and dependencies
4. Read RESEARCH.md for context
5. Write section in .md file with inline comments
6. **Check against STYLE_GUIDE.md checklist** before finishing
7. Update TASKS.md marking as in-progress → completed
8. Run `mdbook serve` locally to preview

### Workflow B: Diagram Creation Session

1. Pick diagram task (T3.1, T3.2, etc.)
2. Read DIAGRAMS.md for requirements
3. Check plan/SUB_AGENT_GUIDE.md for Mermaid setup (if first time)
4. Create Mermaid code in mdBook markdown fence
5. Insert into chapter .md file with explanation text
6. Update DIAGRAMS.md marking as in-progress → completed
7. Test with `mdbook serve`

### Workflow C: Example Code Session

1. Pick code task (T2.1, T2.2, etc.)
2. Read RESEARCH.md for async_blink context and API info
3. **NEW**: Check `OpenMRNLite/examples/ESP32/` for working patterns to adapt (now available locally!)
4. Consider launching sub-agent for research (SA1, SA2, SA4 from SUB_AGENT_GUIDE.md) — SA3 is now optional
5. Write .ino sketch or platformio.ini with detailed comments
6. Test on hardware if available
7. Update TASKS.md with completion status
8. Document any findings in RESEARCH.md

### Workflow D: Research-Heavy Session

1. Identify what info is missing (check RESEARCH.md unknowns)
2. Review SUB_AGENT_GUIDE.md for matching sub-agent task (SA1-SA5)
3. Launch sub-agent with appropriate prompt
4. Wait for results, synthesize into findings
5. Update RESEARCH.md with output
6. Use output to unblock implementation tasks

---

## Workflow: GPIO Hardware Integration (Current Phase)

**Phase 6 is a multi-session hardware integration effort**: Configuration patterns (T6.1 ✅) → Single I/O (T6.2-T6.3) → Multiple I/O (T6.4) → Chapter wrap-up (T6.5)

**Standard workflow for GPIO tasks:** All code changes follow this pattern:
1. **Update config.h first**: Add CDI entries, consume reserved space, update CANONICAL_VERSION only if total size changes
2. **Update main.cpp second**: Add GPIO pins, create producer/consumer instances, update factory_reset()
3. **Test on hardware**: Build, upload, verify with JMRI
4. **Document**: Write corresponding chapter section with code diffs and wiring diagrams (follow STYLE_GUIDE.md)

**Best Practice - Use Sub-Agents for Research:**
- For intensive research tasks (e.g., searching OpenMRNLite source code, understanding API patterns), launch a sub-agent
- This preserves token budget in the main session for file editing and content writing
- Research findings from sub-agent can be summarized and applied directly to code/docs

### Session 1: Configuration Evolution (T6.1) ✅
✅ Completed 2025-12-30
   - Added `blink_enabled` configuration with Enabled/Disabled mapping
   - Added 32 bytes of reserved configuration space
   - Incremented CANONICAL_VERSION to 0x0003 (one-time factory reset)
   - Created src/06-gpio/01-enable-disable.md documentation
   - Updated src/06-gpio/overview.md for two-part chapter structure
   - Tested on hardware with JMRI

### Session 2: Add Single Button Producer (T6.2) — NEXT
**Ready to start**. Add physical button input with GPIO producer:
   - Wire button to GPIO 18 (breadboard + pullup)
   - Consume reserved space (32→20 bytes) for ProducerConfig
   - Create ConfiguredProducer instance with RefreshLoop polling
   - Test with JMRI: press button, see events produced
   - **DO NOT increment CANONICAL_VERSION** (reserved space pattern)

### Session 3: Add Single LED Consumer (T6.3) — FUTURE
   - Wire LED to GPIO 19 (breadboard + resistor)
   - Consume reserved space (20→8 bytes) for ConsumerConfig
   - Create ConfiguredConsumer instance
   - Test cross-wiring via JMRI (configure LED to respond to button events)
   - Document single-button-led.md with complete wiring and code walkthrough

### Session 4: Scale to Multiple I/O (T6.4) — FUTURE
   - Add second button (GPIO 21) and second LED (GPIO 22)
   - Use RepeatedGroup pattern for scalable configuration
   - Implement MultiConfiguredConsumer for memory efficiency
   - Document scaling-multiple-io.md

---

## Files You'll Edit Frequently

| File | Edit When | Why |
|------|-----------|-----|
| plan/TASKS.md | Starting/ending tasks | Track progress |
| plan/PROJECT_STATUS.md | End of session | Summarize accomplishments |
| test/async_blink_esp32/include/config.h | Adding GPIO config | CDI definitions |
| test/async_blink_esp32/src/main.cpp | Adding GPIO code | Hardware integration |
| src/06-gpio/*.md | Writing GPIO docs | Chapter 6 content |
| plan/RESEARCH.md | Finding new info | Document discoveries |

---

## Key Constraints & Reminders

✅ **Do This**:
- Keep sentences in content clear and concise
- Include inline code comments in .ino and platformio.ini
- Create Mermaid diagrams for abstract concepts
- Test locally with `mdbook serve` before finalizing
- Link to markdown/standards/ for deep technical references
- Use sub-agents for research-heavy tasks

❌ **Avoid This**:
- Don't include Nucleo/STM32 content in v0.1 (archive it)
- Don't try to explain CAN hardware (defer to future chapter)
- Don't create custom WiFi credential system (hardcode for v1)
- Don't get deep into OpenMRN C++ implementation details
- Don't aim for production-ready code (educational, working example is enough)

---

## Troubleshooting Common Issues

| Issue | Solution |
|-------|----------|
| "Where do I start?" | Read plan/README.md → plan/PROJECT_STATUS.md → pick from TASKS.md |
| "What info do I need?" | Check plan/RESEARCH.md; if missing, consider sub-agent |
| "Mermaid not rendering" | Check mdBook config in book.toml; test with `mdbook serve` |
| "GPIO conflict with WiFi?" | See SUB_AGENT_GUIDE.md SA4 or search ESP32 GPIO allocation docs |
| "How does JMRI work?" | See SUB_AGENT_GUIDE.md SA2 or plan/RESEARCH.md |
| "Can't find async_blink code?" | It's in openmrn/applications/async_blink/; see plan/RESEARCH.md for details |

---

## Success Criteria for v0.1 Completion

✅ Book successfully builds with `mdbook build` (no errors)
✅ Chapter 1 (Intro) has Mermaid startup sequence + event state machine diagrams
✅ Chapter 2 (Getting Started) updated with ESP32 context and platform overview
✅ Chapter 3 (Your First WiFi-Based OpenLCB Node) complete with CDI and JMRI monitoring
✅ Chapter 4 (Configuration & Persistence) complete with configuration patterns and versioning
✅ Chapter 5 (Switching to CAN Hardware) complete with hardware wiring and JMRI setup
⏳ Chapter 6 Part 1 (Configuration Evolution) complete - Part 2 (GPIO hardware) in progress
⏳ Chapter 6 Part 2 (GPIO/Physical I/O): Button/LED integration with ConfiguredProducer/Consumer
✅ Nucleo/STM32 content archived (not in main TOC)
⏳ Code examples tested and working on ESP32 hardware
✅ All plan/ tracking files updated

---

## Questions to Ask Before Starting Next Session

- Do you have an ESP32 board to test with? (If yes, can verify hardware wiring)
- Which specific ESP32 board variant? (DevKit v1, M5Stack, Feather, etc.?)
- Will you write the async_blink_esp32 sketch yourself or want me to write it?
- Should we create the diagrams before or after writing content?
- Any blockers from last session we need to resolve?

---

**Last Updated**: 2025-12-30  
**Last Edit**: Updated for Phase 6 GPIO implementation (T6.1 complete, T6.2 next)  
**Ready For**: T6.2 - Add Single Button Producer with ConfiguredProducer
