# Monitoring & Verification

Building an OpenLCB node is only half the battle. You also need **visibility** into what your node is doing. This section introduces the monitoring tools and how to verify your node is working correctly.

## JMRI: The Essential Monitoring Tool

**JMRI** (Java Model Railroad Interface) is a free, open-source tool for model railroad control and monitoring. It's essential for OpenLCB development:

**Why JMRI?**
- **Message decoder** - Translates raw hex into readable OpenLCB messages
- **Network monitor** - See all CID, RID, AMD, event messages in real-time
- **Testing tool** - Send events to your node and verify responses
- **Layout integration** - Connect your node to a larger LCC network
- **Configuration editor** - Edit node settings (via CDI) without recompiling firmware

For OpenLCB development, JMRI is **invaluable** for seeing what's happening on the network.

## TCP/GridConnect Protocol

When your ESP32 node runs a TCP server (on port 12021 by default), JMRI connects as a TCP client. The communication uses **GridConnect ASCII format**, which is human-readable:

```
:X18AD4000N;
:X19B84000N;
:X1CED4000N;
:X1080C000N;
```

Each line is an OpenLCB message. The format is:
- `:X` - GridConnect header
- `18AD4000` - OpenLCB header and data (hex)
- `N` - Indicates normal message (not error)
- `;` - Message terminator

**What You'll See**: When your node starts up, JMRI shows:
1. **Four CID frames** (checking alias availability)
2. **RID frame** (reserving an alias)
3. **AMD frame** (mapping Node ID to alias)
4. **Initialized message** (node is online and ready)
5. **Producer/Consumer Identified messages** (node capabilities)
6. **Event reports** (button presses, LED changes, etc.)

This startup sequence takes a few milliseconds and happens automatically.

## Running a Local TCP Hub

Your ESP32 node runs **both** a node (producing/consuming events) **and** a hub (routing messages) simultaneously. The hub is a simple TCP server:

- Listens on port 12021 by default
- Accepts connections from JMRI, other nodes, and monitoring tools
- Forwards all OpenLCB messages between participants
- Requires just a few lines of OpenMRN-Lite code to set up

This is a key design pattern: a single device can be both a node and a hub, which is perfect for development and small layouts.

## Quick Verification Steps

When your node is running:

1. **Serial monitor** - See startup messages and debug output
2. **JMRI connection** - Connect JMRI to `localhost:12021` (or your ESP32's IP address)
3. **Message trace** - Watch the four startup frames (CID/RID/AMD/Init)
4. **Event test** - Produce and consume a test event; watch it appear in JMRI
5. **Node properties** - View your node's name and description (from SNIP data)

Chapter 4 includes detailed screenshots and step-by-step JMRI configuration instructions.

## What's Next

With monitoring tools in place, you're ready to:
1. Install PlatformIO (Chapter 3)
2. Build and deploy the async_blink example
3. Verify it with JMRI
4. Start understanding the code

Let's get hands-on in the next chapter.
