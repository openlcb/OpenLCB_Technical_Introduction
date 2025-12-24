# Forward References & Link Updates

## Purpose

This file tracks all forward references in the book to future chapters that don't yet exist. When those chapters are written, these references should be updated with actual links.

**Format**: `[Location]` | `[Forward Reference Text]` | `[Target Chapter]` | `[Status]`

---

## Current Forward References to Update

### intro.md

| Location | Current Text | Target Chapter | Status | Notes |
|----------|--------------|-----------------|--------|-------|
| Transport Layers section | "Future chapters will cover adding CAN hardware" | Adding CAN Hardware (future) | ⏳ Not yet written | Migrating from WiFi/TCP to CAN bus transport |
***Forward References & Link Updates***

## Purpose

Track forward references in the book that point to chapters not yet written. When a target chapter exists, update the source file to link to the new chapter and mark the reference DONE.

**Row format used below**: `[Location]` | `[Forward Reference Text]` | `[Target Chapter Title]` | `[Status]` | `[Notes]`

---

## Current Forward References to Update

### intro.md

| Location | Current Text | Target Chapter Title | Status | Notes |
|----------|--------------|----------------------|--------|-------|
| Transport Layers section | "Future chapters will cover adding CAN hardware" | Switching to CAN | ⏳ Not yet written | Migrate WiFi/TCP discussion to CAN chapter when written |

### start.md

| Location | Current Text | Target Chapter Title | Status | Notes |
|----------|--------------|----------------------|--------|-------|
| "Understanding OpenMRN-Lite" section | "See Chapter 3 for deeper dive" | OpenMRN-Lite Architecture & Capabilities | ✅ Linked | Reference resolved to `src/03-esp32-arduino/esp32-arduino.md` |

### intro_async_blink.md (Node Startup Sequence)

| Location | Current Text | Target Chapter Title | Status | Notes |
|----------|--------------|----------------------|--------|-------|
| End note | "Future chapters will dive deeper into alias negotiation and collision handling" | Understanding Startup Sequence (detailed) | ⏳ Not yet written | Deep CID/RID/AMD negotiation details belong here |

### intro_ab_events.md (Events and Run Mode)

| Location | Current Text | Target Chapter Title | Status | Notes |
|----------|--------------|----------------------|--------|-------|
| End note | "Future chapters will show node examples with button inputs and LED outputs" | ESP32 with Arduino & PlatformIO | ✅ Linked | Content exists in `src/03-esp32-arduino/esp32-arduino.md` |

### esp32-arduino.md (What's Next / End notes)

| Location | Current Text | Target Chapter Title | Status | Notes |
|----------|--------------|----------------------|--------|-------|
| What's Next section | "Future chapters will explore: Adding CAN hardware, Advanced events, Memory config, SNIP, Train control" | Various planned chapters (see Planned Future Chapters below) | ⏳ Not yet written | Break into separate planned chapters and add links when available |

---

## Planned Future Chapters (for link targets)

- Configuration & Persistence — `configuration-persistence.md`
- Switching to CAN — `switching-to-can.md`
- Physical I/O / GPIO hardware — `gpio-hardware.md`
- Understanding Startup Sequence (detailed)
- Advanced Events, Node Configuration, Memory Configuration, etc.

---

## Link Update Workflow

When a new chapter is created:
1. Find forward references in this file that target the chapter
2. Update source files replacing placeholder text with markdown links to the new chapter
3. Update this file's Status column to `✅ Linked`

Example:
```markdown
OLD: "See Chapter X: Understanding Alias Negotiation for details"
NEW: "See [Understanding Alias Negotiation](../understanding-startup-sequence.md) for details"
```

---

## Current Status Summary

- ⏳ Not Yet Written: references targeting planned chapters (see table above)
- ✅ Linked: references already resolved to existing chapters

---

## Future Review Points

- After creating a new chapter, run a pass to replace placeholder references and mark them linked here
- Keep this file minimal and focused on mapping placeholders → target chapter titles (avoid numeric chapter IDs)

---

**Last Updated**: 2025-12-24
**Purpose**: Keep forward references manageable and unambiguous; prefer chapter titles/paths over numeric IDs

