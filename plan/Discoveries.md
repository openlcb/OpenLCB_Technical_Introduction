This document contains information about discoveries I've made since creating the esp32-arduino.md document.

---

#  JMRI Version History: LCC Support Evolution  
*Why DecoderPro's behavior changed between JMRI 4.x and 5.x*

## What Changed
Yes  DecoderPro's LCC "rename node" behavior **did change**, and some functionality effectively regressed when LccPro was introduced.

### In JMRI 4.x:
- DecoderPro **did** allow changing the node's displayed name
- That name was stored in **JMRI's internal LCC node table** (not on the node itself)
- It did **not** write to the node's ACDI memory
- It was purely a **JMRI-side alias** for convenience

### In JMRI 5.x (after LccPro was introduced in JMRI 5.11.1):
- LccPro was introduced as the **authoritative LCC configuration tool**
- LccPro **writes names into ACDI User Name** (directly on the node)
- DecoderPro now **reads** that ACDI name from the node
- DecoderPro's old **local alias mechanism was removed or deprecated**
- DecoderPro **no longer provides a way to rename nodes**

### The Result:
- Changing the name in **LccPro**  updates ACDI on the node  both tools show the new name
- Changing the name in **DecoderPro**  no longer works (the UI doesn't write ACDI anymore)

The first stable release of JMRI with LccPro was 5.12, and therefore that is the minimum version recommended for this guide.

---

#  JMRI LCC Quirks and Known Behaviors  
*Why LccPro and DecoderPro sometimes show different results*

JMRI has two different user interfaces that can configure LCC/OpenLCB nodes:

- **LccPro**  the newer, LCC-native tool (introduced JMRI 5.11.1)  
- **DecoderPro**  the older, DCC-oriented tool that has LCC features added on top  

When LccPro was introduced in JMRI 5.x, the developers shifted LCC configuration responsibilities into the new tool. DecoderPro's LCC UI was **not** updated to match LccPro's new behavior, so the two tools now behave very differently.

**Bottom line**: LccPro is the authoritative LCC configuration tool for JMRI 5.x. DecoderPro's LCC dialogs are legacy UI with incomplete functionality.

This section explains the differences so you know what to expect  and so you don't waste time debugging firmware that is actually working correctly.

---

##  1. Identification Section (SNIP)  
**Symptom:**  
The "Identification" section (Manufacturer, Model, Hardware, Software) appears **blank** when opened from DecoderPro, but shows correct values in LccPro.

**Cause:**  
DecoderPro's configuration dialog does **not** always request or display SNIP (Simple Node Information Protocol) data. LccPro does.

**Status:**  
This is a known limitation in DecoderPro's LCC UI.

**Workaround:**  
Use **LccPro** to view or verify SNIP information.

---

##  2. ACDI User Info (Name and Description)  DecoderPro Cannot Edit  
**Symptom:**  
You can view the Node Name and Description in DecoderPro, but the fields are **read-only** or the UI provides no way to edit them.

**Cause:**  
This is **intentional by design in JMRI 5.x**. DecoderPro no longer provides a UI to edit ACDI values. DecoderPro's old "local alias" mechanism from JMRI 4.x was removed when LccPro became the authoritative LCC configuration tool.

DecoderPro **can still read** ACDI values (showing whatever LccPro or the node's firmware wrote), but it cannot write them back.

**Status:**  
This is a permanent architectural change in JMRI 5.x, not a bug.

**Workaround:**  
Use **LccPro** to edit node names and descriptions.

---

##  3. CDI Rendering Differences  
**Symptom:**  
The same CDI may appear slightly different between DecoderPro and LccPro:

- Some segments appear collapsed or missing  
- Some fields don't refresh after writes  
- Some names or descriptions don't update until restart  

**Cause:**  
LccPro uses the newer CDI rendering engine.  
DecoderPro uses an older panel builder that predates LCC support.

**Workaround:**  
Use **LccPro** for CDI-heavy nodes or when testing new configuration structures.

---

##  4. Event-Based Tools Behave Differently  
**Symptom:**  
Sensors, turnouts, or event monitors behave consistently in LccPro but inconsistently in DecoderPro.

**Cause:**  
DecoderPro's event-handling code is older and not fully aligned with the LCC event model. LccPro uses the newer event framework.

**Workaround:**  
Use **LccPro** when testing event producers/consumers.

---

##  5. Bottom Line  
If you are developing or debugging an LCC/OpenLCB node  especially one using OpenMRN or OpenMRN-Lite  **LccPro is the authoritative tool** for:

- SNIP  
- ACDI  
- CDI  
- Event monitoring  
- Node configuration  

DecoderPro's LCC dialogs are functional but incomplete, and may not display all information correctly.

---

#  SNIP vs ACDI in LCC Nodes  
*A practical explanation for developers, including JMRI limitations*

LCC nodes expose two different kinds of identity information: **SNIP** and **ACDI**. These serve different purposes, live in different parts of the node's memory, and are treated differently by JMRI. Understanding the distinction is essential when building or debugging OpenMRN/OpenMRN-Lite nodes.

---

##  SNIP  Simple Node Information Protocol  
**SNIP defines the device's manufacturer identity.**  
It is static, compiled into the firmware, and cannot be changed from JMRI.

### SNIP includes:
- Manufacturer  
- Model  
- Hardware version  
- Software version  

### Purpose:
- Identifies *what the device is*  
- Used by tools to classify nodes  
- Appears in the **LccPro node list**  
- Appears in the **Node Info pane** (the boxes under the list)

### Key property:
 **SNIP is immutable at runtime**  
 **SNIP is the only data used to label nodes in the LccPro list**

If you have two identical devices, they will appear identical in the node list unless you change the firmware.

---

##  ACDI  Abbreviated Configuration Description Information  
**ACDI defines the user-editable identity of the node.**  
These values live in the node's configuration memory and *can* be changed from JMRI.

### ACDI includes:
- User Name  
- User Description  

### Purpose:
- Describes *what the device means on your layout*  
- Helps with documentation, automation, and layout logic  
- Appears in the **Configure Node dialog**  
- Saved in the node's configuration memory

### Key property:
 **ACDI is editable**  
 **ACDI does NOT change the node list name in LccPro**

This is the most common point of confusion.

---

##  Why the node list doesn't show ACDI values  
The LCC standard intentionally separates:

- **Device identity** (SNIP)  
- **Layout identity** (ACDI)

The node list is meant to answer:

> "What kind of device is this?"

not:

> "What did the layout owner name this device?"

So even if you change the User Name and Description in ACDI, the node list will still show:

- Manufacturer  
- Model  
- Hardware  
- Software  

from SNIP.

To distinguish two identical nodes, you must use:

- Node ID  
- The Configure dialog (which shows ACDI)  

This is expected behavior.

---

##  JMRI Version Differences: What Changed in JMRI 5.x  
JMRI's LCC support is now split between two tools:

###  LccPro (JMRI 5.11.1+)  Authoritative LCC Configuration
- Fully displays SNIP  
- Fully displays ACDI  
- **Can edit ACDI values** (writes directly to node's configuration memory)
- Correctly refreshes values  
- Correctly shows Identification and User Info
- Uses the modern LCC framework

###  DecoderPro (JMRI 5.x)  Legacy LCC Support
- May not show SNIP in the Configure dialog  
- **Can read ACDI but cannot edit it** (legacy UI, no write capability)
- Uses older UI code not fully aligned with LCC  
- Can show blank Identification fields even when the node is correct
- **Cannot rename nodes** (this worked in JMRI 4.x with local aliases, but was removed)

### JMRI 4.x (older installations  if you still have one):
- DecoderPro **could** rename nodes via local JMRI-side aliases
- Those aliases were **not** written to the node's ACDI memory
- LccPro did not exist yet

This is why you may remember being able to rename nodes in DecoderPro  that capability was real in JMRI 4.x, but was removed in JMRI 5.x.

Your firmware is not at fault  this is simply how JMRI 5.x is designed.

---

##  Summary  
- **SNIP** = device identity (static, firmware-defined)  
- **ACDI** = user identity (editable, layout-defined)  
- **Node list uses SNIP only**  
- **Configure dialog shows ACDI**  
- **LccPro fully supports both**  and can edit ACDI
- **DecoderPro can read ACDI**  but cannot edit it

This distinction is fundamental to LCC and explains why editing User Info does not change how nodes appear in the main list. In JMRI 5.x, use **LccPro** to modify node configuration.

---

If you want, I can also help you add a short "Best Practices for Naming Nodes" section to guide users on how to label devices in a layout-friendly way.
