# Your First WiFi-Based OpenLCB Node

This chapter covers everything needed to build and deploy your first OpenLCB node on an ESP32 microcontroller using Arduino and PlatformIO, connected via WiFi.

## Overview

The ESP32 is a powerful, affordable microcontroller with built-in WiFi connectivity, making it ideal for learning OpenLCB concepts. This chapter walks through building a working OpenLCB node that simultaneously runs the node protocol stack and hosts a TCP Hub—allowing JMRI to monitor and control your WiFi-connected node over a network. 

Unlike "real" OpenLCB networks (which typically use CAN hardware transport), this WiFi-based approach lets you get a functioning node running quickly without special hardware, and it's perfect for embedded systems with network connectivity.

As described in the "Network Architecture" section of Chapter 2, your ESP32 will act as both:
- An **OpenLCB node** that produces and consumes events
- A **TCP Hub** listening on port 12021 for JMRI connections

We'll be using the **OpenMRN-Lite library**, which is the Arduino version of OpenMRN. If you're curious about why OpenMRN-Lite specifically, or what it can (and can't) do, see Chapter 2.2 ("OpenMRN-Lite Architecture & Capabilities") for a deeper dive. For now, know that it's the right tool for the job and it has everything we need to build real, functional OpenLCB nodes.
