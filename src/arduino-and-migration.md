# Arduino for Phases 1–2 (Migration Path for Phase 3)

The first decision in building an OpenLCB node is: **which platform and framework?** For phases 1–2, we use Arduino because it's the most accessible starting point for learning and offers excellent real-world functionality. Phase 3 introduces the migration path to full FreeRTOS-based solutions only if you need advanced features.

## Why Arduino First?

Arduino provides a dramatically lower learning curve and barrier to entry compared to FreeRTOS or ESP-IDF:

- **Minimal setup time**: You can have working code running in minutes without complex build systems or operating system knowledge
- **Large ecosystem**: Thousands of tutorials, libraries, and community examples
- **Rapid iteration**: Fast compile/upload cycle (with PlatformIO)
- **Breadboard-friendly**: Ideal for learning with hobby microcontrollers
- **No RTOS complexity**: Single-threaded execution; no threading, mutexes, or scheduler concepts needed

When learning OpenLCB, you want to focus on **understanding the protocol and building real I/O functionality**—not spending weeks learning RTOS concepts and debugging multi-threaded interactions. Arduino (with OpenMRN-Lite) lets you get working nodes immediately.

## Arduino + OpenMRN-Lite: Phases 1–2

For phases 1–2 (TCP quick-start and CAN-based I/O), you use **OpenMRN-Lite**, the Arduino version of OpenMRN.

OpenMRN-Lite is **production-quality and fully functional**. It supports:
- **CDI configuration** - Configure nodes without recompiling
- **CAN bus** - Full CAN transport with proper arbitration
- **Event producers/consumers** - The core OpenLCB pattern
- **Persistent storage** - SPIFFS/SD card for configuration
- **Real-time monitoring** - JMRI integration via TCP or CAN

**This is not a compromise.** Most hobby and DIY LCC installations use OpenMRN-Lite for years. You're not learning a dead-end; you're choosing the right tool for real-world I/O projects.

## Do You Need Phase 3?

Most projects stay in phases 1–2. Phase 3 (FreeRTOS + Full OpenMRN) is only for advanced scenarios:

- **Command stations and bridges** — Complex routing between multiple transports
- **Virtual nodes** — Multiple logical nodes on one device
- **Traction protocol** — Locomotive decoder features
- **Multi-threaded hubs** — Complex systems requiring background threads and advanced concurrency

If you're building sensors, I/O controllers, or simple nodes for a layout, phases 1–2 have everything you need. Most hobby and model railroad installations never require phase 3.

## If You Need Phase 3 Later

If you eventually need phase 3 features, the foundation you built in phases 1–2 makes the path forward straightforward:

- Your OpenLCB **protocol knowledge transfers directly** — message formats, event semantics, CDI structures all stay the same
- You'll switch from Arduino to **ESP-IDF** (Espressif's full RTOS environment) and use full **OpenMRN** library instead of OpenMRN-Lite
- You'll add FreeRTOS-aware code (threading, message queues, etc.)
- The core OpenLCB concepts you've mastered (producers/consumers, startup sequence, events) remain unchanged—you're adding threading sophistication on top of a solid foundation

## For Phases 1–2

We'll use **Arduino + OpenMRN-Lite**. This combination gives you:
- A working TCP node in minutes (phase 1)
- A fully-featured CAN node for real layouts (phase 2)
- Time to focus on OpenLCB concepts instead of RTOS complexity
- A solid foundation for phase 3 if you ever need it

Let's get started with simplicity, solid fundamentals, and the knowledge that you can expand whenever you need to.
