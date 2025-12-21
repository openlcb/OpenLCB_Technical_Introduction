# OpenMRN-Lite Architecture Clarification (2025-12-21)

## ✅ The Accurate Short Answer

**For ESP32 with Arduino/PlatformIO: Use OpenMRN-Lite. It IS the Arduino version of OpenMRN.**

This document clarifies common misconceptions about OpenMRN vs OpenMRN-Lite and explains the architecture.

---

## ✅ The Critical Insight (Clears Up Everything)

**OpenMRN-Lite is not a "lite" or "cut-down" version in the sense of less capable.**

**OpenMRN-Lite IS the Arduino version of OpenMRN, built for single-threaded, no-RTOS environments.**

They are the same codebase, compiled with different options:
- **Full OpenMRN**: Uses FreeRTOS + POSIX threading, multi-transport bridging, virtual nodes (requires Linux/macOS/Windows/native ESP-IDF)
- **OpenMRN-Lite**: Single-threaded executor, no RTOS required, Arduino-compatible (Arduino IDE, PlatformIO on ESP32)

---

## ✅ Why Arduino Cannot Run Full OpenMRN

The Arduino runtime environment:
- ❌ No POSIX pthreads API
- ❌ No native FreeRTOS integration
- ❌ No multi-transport scheduler
- ❌ No virtual node host

Full OpenMRN requires all of these. Therefore:
- **You cannot run full OpenMRN in Arduino.**
- **OpenMRN-Lite is the only OpenMRN version for Arduino/ESP32.**
- There is no "full OpenMRN for Arduino" to migrate to.

If you need full OpenMRN features on ESP32, you must:
- Use native ESP-IDF (not Arduino framework)
- Write in C/C++ with native FreeRTOS APIs
- Not use the Arduino IDE or PlatformIO's Arduino environment

---

## ✅ What OpenMRN-Lite Supports (You might think it doesn't)

✅ CDI (Configuration Description Information)  
✅ Event producers/consumers  
✅ Datagrams  
✅ SNIP (Simple Node Info Protocol)  
✅ ACDI (Abbreviated Configuration Description Information)  
✅ CAN transport (optional, requires transceiver)  
✅ TCP Hub (optional, WiFi-based)  
✅ Factory reset patterns  
✅ Configuration memory management  

The IOBoard.ino examples in BOTH the `openmrn/arduino/examples/ESP32/` AND `openmrnlite/examples/ESP32/` folders are identical because they both use OpenMRN-Lite with full CDI.

---

## ✅ What OpenMRN-Lite Does NOT Support

❌ Virtual nodes (hosting multiple logical nodes)  
❌ Traction protocol (command station features)  
❌ Multi-transport bridging (CAN ↔ TCP ↔ USB routing)  
❌ Multi-threaded executor (all I/O is single-threaded)  
❌ Hub services (virtual topology management)  
❌ Extensive memory configuration patterns  

These features require FreeRTOS and POSIX threading, which Arduino doesn't provide.

---

## ✅ This Book's Decision (Clarified 2025-12-21)

**We use OpenMRN-Lite (the Arduino version) because it's the correct and only choice for Arduino/ESP32.**

We enhance it by:
1. Enabling CDI support (library already supports it; our example disabled it)
2. Hardcoding configuration in v0.1 (for simplicity)
3. Making configuration dynamic in Chapter 5 (via JMRI CDI configuration)

**This is not a limitation of OpenMRN-Lite. It's a design choice for progressive learning.**

---

## ✅ Decision Matrix: When to Use What

| Need | Use | Why |
|------|-----|-----|
| Learning LCC on ESP32 | OpenMRN-Lite | Single-threaded, Arduino-native, supports CDI |
| Building a sensor node (no threads) | OpenMRN-Lite | Minimal code, small footprint, proven pattern |
| Fixed-function turnout controller | OpenMRN-Lite | No config changes needed; extremely stable |
| Virtual nodes on ESP32 (hosting multiple nodes) | ❌ Not possible | Requires FreeRTOS; switch to ESP-IDF + full OpenMRN |
| Command station / traction protocol | ❌ Not possible | Requires full OpenMRN + FreeRTOS; use Linux/Windows |
| Multi-transport bridging (CAN ↔ TCP) | ❌ Not possible | Requires full OpenMRN + FreeRTOS; use Linux/Windows |

---

## ✅ If You're Reading Online Documentation

If you see documentation that says:
- "Use full OpenMRN for learning and OpenMRN-Lite only for embedded" — **this is outdated or misleading**
- "OpenMRN-Lite doesn't support CDI" — **false, it does (see IOBoard examples)**
- "Switch from OpenMRN-Lite to full OpenMRN when your needs grow" — **not possible in Arduino; requires switching toolchains**

The accurate modern recommendation is:
- **For Arduino/ESP32**: Use OpenMRN-Lite with CDI enabled
- **For FreeRTOS/native features**: Switch to ESP-IDF + full OpenMRN (different toolchain entirely)

---

## ✅ Quick Reference: Architecture Comparison

| Aspect | OpenMRN-Lite | Full OpenMRN |
|--------|--------------|--------------|
| **Threading Model** | Single-threaded executor | FreeRTOS multi-threaded |
| **Platform** | Arduino (ESP32, STM32) | Linux, macOS, Windows, ESP-IDF |
| **CDI Support** | ✅ Yes | ✅ Yes |
| **Event Producer/Consumer** | ✅ Yes | ✅ Yes |
| **Datagram Support** | ✅ Yes | ✅ Yes |
| **CAN Transport** | ✅ Optional | ✅ Integrated |
| **TCP Hub** | ✅ Optional | ✅ Integrated |
| **Virtual Nodes** | ❌ No | ✅ Yes |
| **Traction Protocol** | ❌ No | ✅ Yes |
| **Multi-Transport Bridging** | ❌ No | ✅ Yes |
| **Memory Footprint** | Small (~50KB code) | Large (~2MB+) |
| **Abstraction Level** | Sketch-friendly | System library |
| **Best For** | Learning, sensors, controllers | Complex systems, servers |
