# OpenLCB Standards & Technical Notes References

This document maintains a curated collection of NMRA OpenLCB standards and technical notes. Use these links when writing chapters to add proper citations.

## How to Use This Document

When adding references to a chapter:
1. Find the relevant standard/TN below
2. Copy the link and add it to your chapter's "References" or "Further Reading" section
3. Update this document if you discover new relevant standards

---

## CAN Frame Transfer

Used for: Node startup sequence, alias reservation, CID/RID/AMD frames, alias collision handling, conflict resolution

**Standard (S-9.7.2.1)**
- Title: OpenLCB CAN Frame Transfer Standard
- Date: 2024-07-22
- URL: https://www.nmra.org/sites/default/files/standards/sandrp/LCC/S/s-9.7.2.1-canframetransfer-2024-07-22.pdf
- Key Sections: 
  - 6.2.1 (Reserving Node ID Alias - 200ms wait requirement)
  - 6.2.5 (Alias Collision Handling)
  - 6.2.3 (Node ID Alias validation and invalidation)

**Technical Note (TN-9.7.2.1)**
- Title: OpenLCB CAN Frame Transfer Technical Note
- Date: 2024-07-22
- URL: https://www.nmra.org/sites/default/files/standards/sandrp/LCC/TN/tn-9.7.2.1-canframetransfer-2024-07-22.pdf
- Purpose: Background, examples, and explanatory material for CAN frame specifications

---

## Message Network

Used for: Initialization Complete message, message types, error handling, general network protocol, message timing

**Standard (S-9.7.3)**
- Title: OpenLCB Message Network Standard
- Date: 2024-07-22
- URL: https://www.nmra.org/sites/default/files/standards/sandrp/LCC/S/s-9.7.3-messagenetwork-2024-07-22.pdf
- Key Sections: 
  - 3.3.1 (Initialization Complete message definition)
  - 3.4.1 (Node Initialization interaction)
  - 3.7 (Delays and Timeouts - 750ms message sending requirement)

**Technical Note (TN-9.7.3)**
- Title: OpenLCB Message Network Technical Note
- Date: 2024-07-22
- URL: https://www.nmra.org/sites/default/files/standards/sandrp/LCC/TN/tn-9.7.3-messagenetwork-2024-07-22.pdf
- Purpose: Background, examples, and explanatory material for message network protocol

---

## Unique Identifiers

Used for: Node ID generation, uniqueness requirements, 6-byte identifier format, Node ID structure

**Standard (S-9.7.0.3)**
- Title: OpenLCB Unique Identifiers Standard
- Date: 2024-07-22
- URL: https://www.nmra.org/sites/default/files/standards/sandrp/LCC/S/s-9.7.0.3-uniqueidentifiers-2024-07-22.pdf
- Key Sections: Node ID format and allocation requirements

**Technical Note (TN-9.7.0.3)**
- Title: OpenLCB Unique Identifiers Technical Note
- Date: 2024-07-22
- URL: https://www.nmra.org/sites/default/files/standards/sandrp/LCC/TN/tn-9.7.0.3-uniqueidentifiers-2024-07-22.pdf
- Purpose: Background, examples, and explanatory material for Node ID allocation

---

## By Chapter (current and future)

### Introduction
- **Node section**: [S-9.7.0.3](#unique-identifiers)
- **Node Startup Sequence section**: [S-9.7.2.1](#can-frame-transfer), [TN-9.7.2.1](#can-frame-transfer)
- **Events and Run Mode section**: [S-9.7.3](#message-network)

### Getting Started
- (No specific standards referenced yet; general concepts)

### OpenMRN-Lite Architecture & Capabilities
- (References may be added when covering feature comparisons)

### ESP32 with Arduino & PlatformIO (current example chapter)
- (References may be added when covering WiFi/TCP mapping)

### Physical I/O with GPIO (future / hardware)
- (Future content - will reference CAN standards if needed)

---

## Finding Other Standards

For additional standards not listed here, visit:
https://www.nmra.org/standards-and-recommended-practices

Standards are organized under: Standards → OpenLCB Technical Standards → S-9.7.x series

---

## Notes for Contributors

- Always link to the Standards folder, not the Technical Notes, when requiring normative information
- Link to Technical Notes when providing background, examples, or explanatory material
- When adding new standards, maintain the organized structure (by topic first, then by chapter)
- Update the "By Chapter" section when adding references to chapters
