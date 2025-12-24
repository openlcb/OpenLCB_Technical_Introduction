# Configuration & Persistence

## Overview: Why Configuration Matters

In Chapter 3, we built a working OpenLCB node with a hardcoded node name (`async_blink`) and fixed event IDs. This is perfect for learning—you can compile once and run immediately. But production nodes need flexibility: different users want different names, different event IDs, different blink intervals to match their layout or preferences.

This is where **configuration** comes in. OpenLCB provides a standard way to store and modify node settings without recompiling firmware. Through tools like JMRI, users can:
- Edit the node name and description
- Modify event IDs
- Adjust the blink interval (how fast the LED blinks)
- Change other parameters

All changes persist across reboots, stored safely in the node's flash memory.

## How Configuration Works in OpenLCB

Recall from Chapter 3 that our node exposes three key concepts (detailed in the next section):

| Concept | Purpose |
|---------|---------|
| **ACDI (Automatic Configuration Description Information)** | Standardized data block storing node identity (manufacturer, model, hardware/software version, user name, description) |
| **SNIP (Simple Node Information Protocol)** | Network protocol that broadcasts ACDI data so other nodes and tools can discover and identify this node |
| **CDI (Configuration Description Information)** | Schema describing what configuration options the node has and how to edit them via tools like JMRI |

In this chapter, we'll explore:
1. **Storage Model** — Where configuration lives, how ACDI/SNIP/CDI fit together, and the binary file format
2. **Editing in JMRI** — How to change node name and persist changes
3. **Factory Reset Behavior** — What happens on first boot
4. **Persistence Details** — How data survives reboots and power loss
5. **Adding Configurable Settings** — Make blink interval editable via JMRI
6. **Configuration Versioning** — Evolving your schema safely as you add more settings

## What We're Covering

This chapter combines conceptual understanding with hands-on implementation. We'll:
- **Part 1** (Sections 1-4): Understand how configuration storage works, edit node name via JMRI, see changes persist
- **Part 2** (Sections 5-6): Make the blink interval configurable, implement persistence in code, handle schema versioning

By the end, you'll have a production-ready configuration system that lets users customize your node's behavior through JMRI without recompiling.
