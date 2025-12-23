# Diagram Inventory & Status

## Planned Diagrams for v0.1

### D1: Node Startup Sequence (Mermaid Sequence Diagram) ⭐ HIGH PRIORITY

**Status**: ✅ Completed (2025-12-19)  
**Location**: `intro_async_blink.md` "Node Startup Sequence" section  
**Also referenced from**: `intro_ab_events.md` conceptually  

**Content**: Shows the 4-step startup sequence (CID → RID → AMD → Init Complete)
```
Participant: ESP32 Node
Participant: CAN Bus / Network
Participant: JMRI (Monitor)

Note over ESP32: Startup begins
ESP32->>Bus: CID frame 1 (ID byte 0-1)
ESP32->>Bus: CID frame 2 (ID byte 2-3)
ESP32->>Bus: CID frame 3 (ID byte 4-5)
ESP32->>Bus: CID frame 4 (remaining bits)
Note over ESP32: Pause for collision detection (timeout)
ESP32->>Bus: RID (Reserve ID)
ESP32->>Bus: AMD (Alias Map Definition)
ESP32->>Bus: Initialize Complete
Note over JMRI: Node now online and reachable
```

**Explanation to Accompany**:
- CID = Check ID (4 frames needed due to CAN frame size limits)
- RID = Reserve ID (claims the chosen alias)
- AMD = Alias Map Definition (maps full node ID to short alias)
- Init Complete = Node is ready to receive/send events

**Time to Create**: ~45 minutes  
**Mermaid Version**: Sequence diagram  
**Notes**: This diagram is THE foundation for understanding async_blink startup; must be clear and accurate

---

### D2: CAN Message 29-bit Header Structure (Mermaid Block/Flowchart or ASCII) 🔵 MEDIUM PRIORITY

**Status**: ⏳ Not Started  
**Location**: Insert into `intro.md` after "CAN" section (optional expansion)  
**Purpose**: Explain why CAN frames work this way (foundation for future CAN chapter)

**Content**: Annotated breakdown
```
29-bit CAN Identifier:
┌──────────────────────────────────────────────────┐
│  3 bits  │  4 bits   │  12 bits  │  10 bits     │
│ Priority │Frame Type│  Alias    │  Reserved    │
└──────────────────────────────────────────────────┘
  0-7        0-15        0-4095       0-1023

Example:
0x1A2B3C4D (29-bit)
= Priority 3 (Normal)
+ Frame Type: CID
+ Alias: 0x123
+ Reserved bits
```

**Time to Create**: ~45 minutes  
**Mermaid Version**: Text-based (ASCII or simple block)  
**Notes**: Context for why 4 CID frames needed; can defer if running short on time

---

### D3: async_blink Event Flow (Mermaid Sequence Diagram) ⭐ HIGH PRIORITY

**Status**: ✅ Completed (2025-12-19)  
**Location**: `intro_ab_events.md` "Events and Run Mode" section  
**Purpose**: Shows how events flow between nodes (button → LED interaction)

**Content**: Simple sequence diagram showing button press → event production → LED consumption
```
[Idle]
  ├─ Button pressed
  │   ├─ Produce Event_ID_1
  │   └─ → [Idle]
  │
  ├─ Receive Event_ID_2 (from JMRI)
  │   ├─ Toggle LED
  │   └─ → [Idle]
  │
  └─ (Loop checks every 100ms or so)
```

**Time to Create**: ~30 minutes  
**Mermaid Version**: State diagram  
**Notes**: Keep it simple; focus on event production/consumption flow, not implementation details

---

### D4: OpenMRN Stack Architecture Layers (Mermaid Graph or ASCII) 🟡 OPTIONAL

**Status**: ⏳ Not Started  
**Location**: Optional; could go in end of `esp32-arduino.md` "What's happening under the hood" section  
**Purpose**: Help readers understand layering and how async_blink_esp32 fits

**Content**: Layered architecture
```
┌──────────────────────────────────────────────────┐
│  Application Layer                               │
│  (async_blink_esp32: button input + LED output) │
└──────────────────────────────────────────────────┘
                       ↓
┌──────────────────────────────────────────────────┐
│  OpenLCB Stack (SimpleStack)                     │
│  ├─ Event Producer/Consumer                      │
│  ├─ Node Management                              │
│  ├─ Alias Allocation                             │
│  └─ Message Routing                              │
└──────────────────────────────────────────────────┘
                       ↓
┌──────────────────────────────────────────────────┐
│  Transport Layer (IfTcp for WiFi)                │
│  → GridConnect protocol over TCP/IP              │
└──────────────────────────────────────────────────┘
                       ↓
┌──────────────────────────────────────────────────┐
│  Hardware Layer (ESP32 WiFi Radio)               │
└──────────────────────────────────────────────────┘
```

**Time to Create**: ~30 minutes  
**Mermaid Version**: Graph/flowchart  
**Notes**: Nice to have for conceptual understanding; defer if time-constrained

---

### D5: Breadboard Wiring Schematic (ASCII, Mermaid, or PNG Export) 🔵 MEDIUM PRIORITY

**Status**: ⏳ Not Started  
**Location**: Insert into `esp32-arduino.md` under "Hardware Setup" section  
**Purpose**: Visual guide for button/LED wiring

**Content Options**:
- **Option A**: ASCII art schematic (simple, maintainable)
  ```
  ESP32 Module
  ┌──────────────────┐
  │  GPIO 4 ─────────┼─→ [Breadboard]
  │  GPIO 5 ─────────┼─→ [Breadboard]
  │  GND ────────────┼─→ [Breadboard]
  └──────────────────┘
  
  [Breadboard connections]
  GPIO 4 ──→ Button ──→ GND (with pull-up resistor)
  GPIO 5 ──→ 220Ω Resistor ──→ LED ──→ GND
  ```

- **Option B**: Mermaid graph (if feasible; may be awkward)
- **Option C**: PNG export from draw.io or Fritzing (requires external tool)

**Time to Create**: ~45 minutes (ASCII) to 1.5 hours (PNG)  
**Mermaid Version**: Could be flowchart or graph; ASCII probably simpler  
**Notes**: ASCII art is good enough for v0.1; user can add fancy diagram later

---

## Optional/Future Diagrams (Post-v0.1)

- **D6**: Alias Negotiation Flowchart (CID/RID collision handling)
- **D7**: Multi-Node Startup Timeline (Gantt/timeline showing interleaved initialization)
- **D8**: GridConnect Protocol Format (message examples with annotations)
- **D9**: ESP32 CAN Transceiver Wiring (for future CAN chapter)
- **D10**: JMRI TCP Connection Configuration Screenshot

---

## Diagram Creation Workflow

### Before Creating Diagram

1. **Define Purpose**: What concept does it explain? Who is the audience?
2. **Identify Type**: Sequence? State machine? Architecture? Schematic?
3. **Choose Tool**: Mermaid (if code-based), ASCII (if simple), PNG (if complex)
4. **Write Draft**: Create text/code version first (even for PNG)
5. **Review**: Ensure clarity, correctness, matches narrative

### During Creation

1. **Start Simple**: Get basic structure working, then add details
2. **Test Rendering**: For Mermaid, verify mdBook renders correctly
3. **Add Annotations**: Include labels, legend, or accompanying text
4. **Cross-check**: Verify against standards/documentation

### After Creation

1. **Integrate into Chapter**: Add surrounding explanation text
2. **Cross-reference**: Link from related sections
3. **Version Control**: Commit to git with clear message
4. **Document in DIAGRAMS.md**: Update status to "Done"

---

## Mermaid in mdBook Setup

**Current Status**: mdBook should render Mermaid if properly configured in `book.toml`

**Check Configuration** (in next session):
```toml
[output.html]
# Verify mermaid support is enabled
```

**If Not Enabled**:
1. Add Mermaid preprocessor to `book.toml`
2. Or use fence syntax with `mermaid` language tag:
   ````markdown
   ```mermaid
   graph TD
       A --> B
   ```
   ````

**Testing Diagrams**:
- Use `mdbook serve` to preview locally
- Check that diagrams render properly in HTML output
- If issues, check browser console for JavaScript errors

---

## Diagram Content Checklist

### D1: Startup Sequence
- [ ] Four CID frames shown
- [ ] RID message shown
- [ ] AMD message shown
- [ ] Initialize Complete shown
- [ ] Timing/delay indicated
- [ ] Clear labels for each phase
- [ ] Includes actors (ESP32, Bus, Monitor)

### D2: CAN Header Structure
- [ ] 29-bit ID broken into fields
- [ ] Priority field labeled
- [ ] Frame Type field labeled
- [ ] Alias field labeled
- [ ] Reserved bits labeled
- [ ] Example provided
- [ ] Clear visual separation

### D3: Event State Machine
- [ ] Idle state shown
- [ ] Button press transition
- [ ] Event production
- [ ] Event consumption
- [ ] LED toggle
- [ ] Return to idle
- [ ] Simple and focused

### D4: Stack Layers (if included)
- [ ] Application layer
- [ ] OpenLCB layer
- [ ] Transport layer
- [ ] Hardware layer
- [ ] Clear arrows showing data flow
- [ ] Components labeled

### D5: Breadboard Wiring
- [ ] ESP32 pin labels
- [ ] Button circuit with pull-up
- [ ] LED circuit with resistor
- [ ] GND connections
- [ ] Breadboard layout indicated
- [ ] Pin numbers visible

---

## Status Summary

| Diagram | Type | Priority | Status | Est. Time | Blocker? |
|---------|------|----------|--------|-----------|----------|
| D1: Startup Seq | Mermaid | HIGH | ⏳ Not Started | 45 min | Medium |
| D2: CAN Header | Mermaid/ASCII | MEDIUM | ⏳ Not Started | 45 min | Low |
| D3: Event SM | Mermaid | HIGH | ⏳ Not Started | 30 min | Medium |
| D4: Stack Layers | Mermaid/ASCII | OPTIONAL | ⏳ Not Started | 30 min | Low |
| D5: Breadboard | ASCII/PNG | MEDIUM | ⏳ Not Started | 45 min | Medium |

**Total Time (All Diagrams)**: ~3 hours  
**Time (Must-Have Only)**: ~1.5 hours (D1 + D3)

---

**Last Updated**: 2025-12-18  
**Next Session**: Create D1 and D3 early to unblock content writing
