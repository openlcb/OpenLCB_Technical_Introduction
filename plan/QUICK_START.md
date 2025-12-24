# Quick Start: Next Session Workflow

> NOTE: This file is the immediate session playbook — a short, actionable subset of the canonical backlog in `plan/TASKS.md`.

**Last Updated**: 2025-12-24  
**Current Phase**: Phase 4 In Progress - Configuration & Persistence Chapter

## For Next Session: Start Here

### ⚠️ IMMEDIATE TASK: T4.3 (Configuration Versioning Implementation)
**This session's focus**: Code changes to validate versioning behavior and write Phase 3 documentation

1. **Read These Files First** (10 minutes):
   - **plan/PROJECT_STATUS.md** - Current phase status
   - **plan/TASKS.md** - T4.2 research findings and T4.3 plan
   - **plan/T4.2-RESEARCH.md** - Detailed standards and implementation analysis (START HERE!)
   - **OpenLCB_Technical_Introduction/src/04-configuration/** - All 5 existing Chapter 4 files

2. **Execute T4.3** (2-2.5 hours): Code implementation + Phase 3 documentation
   - Modify test/async_blink_esp32/include/config.h: Add EventInterval configurable field
   - Modify test/async_blink_esp32/src/main.cpp: Implement enhanced apply_configuration()
   - Test on ESP32 hardware: Change interval via JMRI, verify firmware responds
   - Write Phase 3 sections: Versioning, Reserved Space, Version Mismatch, Walkthrough, Best Practices

3. **After T4.3 Works**: Wrap-up (T4.4)
   - Update SUMMARY.md references
   - Add cross-references from Chapter 3
   - Verify mSessions (Completed)
- ✅ T0.1: Restructure intro.md for high-level overview
- ✅ T1.1-T1.4: Initial content restructuring (archive STM32, create ESP32 chapter skeleton)
- ✅ T2.0.1: OpenMRN-Lite CDI enhancement (working code on hardware)
- ✅ T2.6-T2.7: Update Chapter 3 documentation and create OpenMRN-Lite architecture chapter
- ✅ T3.1: Create Mermaid startup sequence diagram
- ✅ T4.1: Write Phase 1 configuration content (5 sections, SUMMARY.md updated)
- ✅ T4.2: Research configuration versioning and standards alignment
- ✅ T2.2: platformio.ini teT4.3)
- **plan/T4.2-RESEARCH.md** - ✨ Comprehensive analysis of standards, implementation, and gaps (CREATED)
- **plan/CHAPTERS.md** - Book structure and content status
- **plan/DIAGRAMS.md** - What diagrams are planned and why
- **plan/STYLE_GUIDE.md** - Writing style & tone to match existing content
- **OpenMRNLite/README.md** - Official library documentation
- **markdown/standards/S-9.7.4.*.md** - OpenLCB standards for reference during Phase 3 writing
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

## Chapter 4 Configuration & Persistence (Multi-Session)

**This is a 3-session effort**: Content writing (T4.1 ✅) → Research (T4.2 ✅) → Implementation & Phase 3 writing (T4.3 READY)

### Session 1: Write Initial Configuration Content (T4.1) ✅
✅ Completed 2025-12-24
   - Created 5 markdown files in src/04-configuration/
   - Updated SUMMARY.md with Chapter 4 TOC
   - Forward references added to Chapter 3

### Session 2: Research & Standards Analysis (T4.2) ✅
✅ Completed 2025-12-24
   - Created comprehensive T4.2-RESEARCH.md document
   - Analyzed OpenLCB standards S-9.7.4.1/2/3
   - Documented OpenMRNLite implementation architecture
   - Identified 6 critical gaps and limitations
   - Verified Chapter 4 content is factually correct

### Session 3: Implementation & Phase 3 Content (T4.3) — NEXT
**Ready to start**. Modify test/async_blink_esp32 to:
   - Add EventInterval configurable field
   - Implement apply_configuration() to read and apply changes
   - Write Phase 3 sections: Versioning, Reserved Space, Best Practices, Hands-on Walkthrough
3. **After T4.1**: Update TASKS.md marking T4.1 complete, ready for Session 2

### Session 2: Research Configuration Versioning (T4.2)
1. **Read These Files First** (10 minutes):
   - plan/TASKS.md T4.2 section
   - plan/RESEARCH.md for async_blink context
   - d:/src/github/LCC/async_blink_esp32_3.3/config.h (how config is currently structured)
2. **Set Up Test Environment** (15 minutes):
   - Use d:/src/github/LCC/test/async_blink_esp32 as working copy (NOT async_blink_esp32_3.3)
   - Ensure it builds and uploads successfully with current code
   - Verify JMRI can connect via TCP
3. **Execute T4.2 Part A** (45 minutes): Add EventInterval field with auto-extend test
   - Update config.h: Add `EventInterval` uint16_t field with reserved space pattern
   - Keep CANONICAL_VERSION = 0x0001 (unchanged)
   - Build/upload to ESP32
   - Observe: Does existing config file auto-extend? Does JMRI show new EventInterval field?
   - Document findings
4. **Execute T4.2 Part B** (45 minutes): Test version bump reset behavior
   - Bump CANONICAL_VERSION to 0x0002
   - Build/upload to ESP32
   - Observe: Is config reset triggered? Does SNIP user data (name) persist? Serial output?
   - Document what happens to ACDI data vs application config
   - Take notes on implications for users
5. **After T4.2**: Document findings in brief research notes, update TASKS.md, mark ready for Session 3

### Session 3: Complete Content & Implement Event Interval (T4.3 & T4.4)
1. **Read These Files First** (10 minutes):
   - plan/TASKS.md T4.3 and T4.4 sections
   - Session 2 findings/notes from Phase 2 research
2. **Execute T4.3 Part A** (1 hour): Write remaining configuration content
   - Add sections to src/04-configuration/configuration-persistence.md:
     - Configuration Versioning behavior (based on Phase 2 findings)
     - Reserved Space Technique patterns
     - Best Practices section
   - Include code examples and JMRI workflow from Phase 2 testing
3. **Execute T4.3 Part B** (1-1.5 hours): Implement configurable EventInterval
   - Update test/async_blink_esp32 config.h: Add EventInterval to CDI (reset CANONICAL_VERSION to 0x0001)
   - Update test/async_blink_esp32 main.cpp: Implement apply_configuration() to read interval
   - Update loop(): Use configurable interval instead of hardcoded EVENT_INTERVAL
   - Add logging to show when config changes detected
   - Test with JMRI: Change EventInterval via Configure, verify firmware reads it and changes behavior
4. **Execute T4.4** (30-45 minutes): Integrate into book
   - Update src/SUMMARY.md: Add Chapter 4 entry
   - Add cross-references from Chapter 3 to Chapter 4
   - Update plan/CHAPTERS.md with Chapter 4 status
   - Run `mdbook build` and verify no errors
5. **After T4.3 & T4.4**: Update TASKS.md marking complete, update PROJECT_STATUS.md with accomplishments

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

**Last Updated**: 2025-12-24  
**Last Edit**: Added Chapter 4 Configuration multi-session workflow (T4.1-T4.4)  
**Ready For**: Chapter 4 Session 1 (T4.1 - Write initial configuration content)
