# Introduction

## Purpose

The purpose of this book is to help you get up to speed on OpenLCB so you can
start creating LCC products. There are a lot of concepts and we'll go into
just enough depth to get you going, without bogging you down with all the
details and possibilites. The standards and the technical notes have a lot
more details.

## Focus of this book

We're going to focus on how to create a device (node) that can be added to an
OpenLCB network. To make learning easier, we'll start with WiFi/TCP transport,
which allows you to see network traffic easily and test without special hardware.
Later chapters will cover adding CAN bus hardware for traditional LCC installations.

## Assumptions

We're assuming you've used LCC products, and therefore already understand
concepts like the producer-consumer model. We also assume you've written
code for microcontrollers and have worked with I/O pins.

## About Names

Let's start with what to some might be confusing—the names LCC vs OpenLCB.
LCC is a brand name owned by the NMRA. It covers a set of standards that have
been adopted by the NMRA for Layout Command Control (hence the LCC).

OpenLCB is the name of the working group that created the standards approved by the NMRA and continues to create new standards. This is a group of dedicated volunteers who are working to fulfill the dream.

You'll notice that the standards use the name OpenLCB everywhere except for the header at the top of the document. In this book, as in the standards, we'll use the term OpenLCB everywhere except this page.

## Transport Layers

OpenLCB is designed to work over different physical transport layers. The two main options are:

**CAN (Controller Area Network)**: The traditional transport for OpenLCB, used in most commercial LCC products. CAN uses a two-wire bus with excellent noise immunity and built-in arbitration, making it ideal for model railroad environments. It requires CAN transceiver hardware and proper bus termination.

**WiFi/TCP**: An alternative transport that uses standard WiFi networking and TCP/IP. This is particularly useful for development and learning because:
- No special hardware required beyond WiFi-capable microcontrollers
- Easy to monitor traffic using standard network tools
- Can connect to JMRI and other tools over your home network
- Simplifies initial prototyping and testing

In this book, we'll start with WiFi/TCP transport because it's more accessible for learning. Once you understand the core concepts, the principles transfer directly to CAN-based implementations. Future chapters will cover adding CAN hardware.

Both transports use the same OpenLCB protocols and message formats—only the physical layer changes.

