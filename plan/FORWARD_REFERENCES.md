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

### start.md (Chapter 2)

| Location | Current Text | Target Chapter | Status | Notes |
|----------|--------------|-----------------|--------|-------|
| "Understanding OpenMRN-Lite" section | "See Chapter 3 for deeper dive" | OpenMRN-Lite Architecture & Capabilities | ✅ COMPLETED | Forward reference to Chapter 3, added 2025-12-21 |

### intro_async_blink.md (Node Startup Sequence)

| Location | Current Text | Target Chapter | Status | Notes |
|----------|--------------|-----------------|--------|-------|
| End note | "Future chapters will dive deeper into how the alias generation algorithm works and how to handle collisions in your code" | Understanding Startup Sequence (future) | ⏳ Not yet written | Detailed CID/RID/AMD collision detection and timeout mechanisms |

### intro_ab_events.md (Events and Run Mode)

| Location | Current Text | Target Chapter | Status | Notes |
|----------|--------------|-----------------|--------|-------|
| End note | "Future chapters will show you how to create your own nodes with real button inputs and LED outputs on an ESP32 microcontroller" | ESP32 with Arduino & PlatformIO | ✅ Chapter exists! | Can be converted to link when esp32-arduino.md content is complete (T2.x tasks) |

### esp32-arduino.md (Chapter 3)

| Location | Current Text | Target Chapter | Status | Notes |
|----------|--------------|-----------------|--------|-------|
| Overview section | "See Chapter 3 for deeper dive into architecture" | OpenMRN-Lite Architecture & Capabilities | ✅ COMPLETED | Forward reference to Chapter 3, added 2025-12-21 |
| What's Next section | "Future chapters will explore: Adding CAN hardware, Advanced events, Memory config, SNIP, Train control" | Various future chapters | ⏳ Not yet written | Multiple forward refs to post-v0.1 content |

---

## Hierarchical Chapter Structure (For Reference)

This shows the conceptual flow and dependencies to help understand when to add links:

```
Chapter 1: Introduction
├─ High-level overview: Nodes, transport (CAN/TCP), startup
├─ → Forward ref to: Chapter X: Understanding Startup Sequence (details)
├─ → Forward ref to: Chapter X: Node Configuration
└─ → Forward ref to: Chapter X: Advanced Events

Chapter 2: Getting Started
├─ Overview of what we're building
├─ Monitoring with JMRI (TCP)
└─ → Forward ref to: Chapter X: Hardware Setup

Chapter 4: ESP32 with Arduino & PlatformIO
├─ PlatformIO, library installation, GPIO basics
├─ → Forward ref to: Chapter X: Adding CAN Hardware
├─ → Forward ref to: Chapter X: SNIP and Configuration
└─ → Forward ref to: Chapter X: Advanced Features

Chapter 5: Understanding Startup Sequence (FUTURE)
├─ Detailed CID/RID/AMD negotiation
├─ Multi-node startup behavior
├─ What other nodes do during startup
└─ Timeout and collision detection

Chapter 6: Adding CAN Hardware (FUTURE)
├─ Transceiver selection (SN65HVD230 vs MCP2551)
├─ GPIO wiring and pin assignments
├─ Bus termination and power considerations
└─ Adapting async_blink_esp32 for CAN

Chapter 7: Node Configuration & SNIP (FUTURE)
├─ Simple Node Information Protocol
├─ CDI (Configuration Description Information)
├─ Making nodes discoverable and configurable
└─ Examples: manufacturer, model, version info

Chapter 8: Advanced Events (FUTURE)
├─ Custom event ID generation and allocation
├─ Producer/consumer patterns
├─ Event filtering and routing
└─ Event data payload handling

Chapter 9: Advanced Features (FUTURE)
├─ Train Control (Traction Protocol)
├─ Broadcast Time
├─ Fast Clock
└─ Other OpenLCB services

Appendix A: Archived Content (FUTURE)
└─ STM32 Nucleo setup and examples (legacy)
```

---

## Link Update Workflow

### When a New Chapter is Created

1. **Find all forward references** pointing to that chapter in this file
2. **Update the links** in the source .md files from placeholder text to actual markdown links
3. **Mark as DONE** in the Status column below
4. **Example change**:
   ```markdown
   OLD: "See Chapter X: Understanding Alias Negotiation for details"
   NEW: "See [Understanding Alias Negotiation](../alias-negotiation.md) for details"
   ```

### Current Status Summary

- ⏳ **Not Yet Written**: 10 forward references across 4 source files
- ✅ **Linked**: 2 (Chapter 3 references from Chapter 2 and Chapter 4)
- 🔄 **In Progress**: 0

---

## Notes on Content Structure

### High-Level vs. Detail Approach

**v0.1 (Current)**:
- Introduction focuses on: nodes, transports (CAN/TCP), startup (what happens, not how)
- Getting started focuses on: practical ESP32 setup and simple example
- Later chapters will provide: deep dives into mechanisms, advanced patterns

**Key Principle**: 
> "Here's what happens during startup. See Chapter X for how the alias negotiation collision detection works."

NOT:
> "Here's the detailed CID frame structure, then RID timeout calculation, then AMD..."

### Multi-Node Participation in Startup

The introduction should mention:
- When one node starts up, other existing nodes may respond with their information
- This is part of the "bus discovery" behavior
- Details on how other nodes participate (listening for CID, responding appropriately) will be in "Chapter X: Understanding Startup Sequence"

---

## Template for Adding New Forward References

When you identify a place where a forward reference should be added:

```markdown
| [file.md - Line X or Section Y] | "See Chapter X: [Title] for [what it covers]" | [Target Chapter Title] (future) | ⏳ Not yet written | [Brief description of content] |
```

---

## Future Review Points

- [ ] Check intro.md for any other places where details could be elevated to overview level
- [ ] Review async_blink_esp32.md (when created) for forward references to future chapters
- [ ] Ensure every forward reference has a clear target chapter in the hierarchy
- [ ] Verify that "Chapter X" placeholders get replaced when chapters are actually written
- [ ] Add links to standards references where appropriate (e.g., "See S-9.7.3-MessageNetwork for...")

---

**Last Updated**: 2025-12-19  
**Created For**: Tracking forward references to future chapters  
**Review Frequency**: After each new chapter is written; update links at that time
