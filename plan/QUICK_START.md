# Quick Start: Next Session Workflow

**Last Updated**: 2025-12-21  
**Current Phase**: Phase 2 In Progress - OpenMRN-Lite CDI Enhancement

## For Next Session: Start Here

### ⚠️ IMMEDIATE TASK: T2.0.1 (OpenMRN-Lite CDI Enhancement)
**This session's focus**: Enhance async_blink_esp32 with CDI support (same library, expanded capability)

1. **Read These Files First** (5 minutes):
   - **plan/PROJECT_STATUS.md** - Strategic decision + current phase
   - **plan/TASKS.md** - T2.0.1 details (immediate priority)
   - **plan/OpenMRN_vs_Lite.md** - Why OpenMRN-Lite is the correct choice for Arduino/ESP32

2. **Execute T2.0.1** (1.5 hours): Code enhancement + hardware testing
   - Update platformio.ini library reference
   - Rewrite config.h with minimal CDI
   - Rewrite main.cpp with Esp32WiFiManager
   - Test on ESP32 hardware

3. **After T2.0.1 Works**: Documentation updates (T2.6 / T2.6.1)
   - Update Chapter 3 for OpenMRN-Lite CDI
   - Create library comparison chapter (Chapter 3) ✅

### Previous Tasks (Completed, No Changes Needed)
- ✅ T2.0: PlatformIO installation guide (update coming)
- ✅ T2.1: async_blink sketch (will enhance with CDI)
- ✅ T2.2: platformio.ini template (no library change needed)
- ✅ T2.4: JMRI TCP setup (no library changes needed)

### 3. Reference Material (As Needed)
- **plan/FORWARD_REFERENCES.md** - ✨ **NEW**: Tracks forward references to future chapters that need link updates later
- **plan/RESEARCH.md** - Key findings about async_blink, ESP32, OpenMRN
  - Includes proper OpenMRNLite usage per README (Library Manager vs. GitHub ZIP vs. local source)
- **plan/CHAPTERS.md** - Book structure and content gaps (UPDATED with restructuring approach)
- **plan/DIAGRAMS.md** - What diagrams are planned and why
- **plan/SUB_AGENT_GUIDE.md** - When to use sub-agents for research
- **plan/STYLE_GUIDE.md** - Writing style & tone to match existing content
- **OpenMRNLite/README.md** - Official library documentation (GPIO restrictions, WiFi setup, CAN details)

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

## Files You'll Edit Frequently

| File | Edit When | Why |
|------|-----------|-----|
| plan/TASKS.md | Starting/ending tasks | Track progress |
| plan/PROJECT_STATUS.md | End of session | Summarize accomplishments |
| src/esp32-arduino.md | Creating new chapter | Main content file |
| src/intro.md | Adding diagrams/content | Update existing sections |
| src/start.md | Refactoring content | Add JMRI TCP section |
| plan/RESEARCH.md | Finding new info | Document discoveries |
| plan/DIAGRAMS.md | Creating diagrams | Track which are done |

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
✅ Chapter 2 (Getting Started) updated with esp32 context and JMRI TCP section
✅ Chapter 3 (ESP32 with Arduino & PlatformIO) is complete with:
   - PlatformIO setup guide
   - Hardware setup with breadboard circuit diagram
   - async_blink_esp32.ino sketch with button/LED
   - platformio.ini template
   - JMRI verification steps
   - Troubleshooting section
✅ Nucleo/STM32 content archived (not in main TOC)
✅ "What's Next" preview section points to future chapters
✅ Code examples are tested and working
✅ All plan/ tracking files updated

---

## Questions to Ask Before Starting Next Session

- Do you have an ESP32 board to test with? (If yes, can verify hardware wiring)
- Which specific ESP32 board variant? (DevKit v1, M5Stack, Feather, etc.?)
- Will you write the async_blink_esp32 sketch yourself or want me to write it?
- Should we create the diagrams before or after writing content?
- Any blockers from last session we need to resolve?

---

**Last Updated**: 2025-12-18  
**Created By**: Initial planning session  
**Ready For**: Next Copilot session
