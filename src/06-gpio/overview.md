# Physical I/O with GPIO

Now that you've successfully created a CAN-based OpenLCB node in Chapter 5, it's time to add physical hardware interactions. But before we wire up buttons and LEDs, we need to establish a configuration pattern that allows your node to grow without losing user settings.

## Chapter Structure

This chapter takes a strategic two-part approach:

### Part 1: Configuration Evolution (No Additional Hardware)

Before adding GPIO pins, we'll enhance the async_blink example with:
- A configurable **enable/disable feature** for event production  
- **Reserved configuration space** for future GPIO expansion  
- **Best practices** for evolving node configuration without triggering factory resets  

**You can complete Part 1 using the existing CAN setup from Chapter 5**—no additional components needed.

This foundation is critical because as we add GPIO features (buttons, LEDs), each new feature requires configuration storage. By reserving space up front, users won't lose their settings when you add new capabilities.

### Part 2: Adding Physical I/O (Additional Hardware Required - Coming Soon)

Once the configuration foundation is solid, we'll add real-world interactions:
- Connect **pushbuttons** to detect physical input  
- Connect **LEDs** to provide visual output  
- Implement OpenLCB **producers and consumers** tied to GPIO pins  
- Scale to **multiple inputs and outputs** using efficient patterns  

## What You'll Learn

By the end of this chapter, you'll understand:

1. **Configuration versioning**: How to evolve your node's settings without breaking existing deployments
2. **Reserved space patterns**: A technique for future-proofing configuration structures
3. **Boolean configuration**: Implementing enable/disable toggles in OpenMRNLite's CDI system
4. **CDI caching behavior**: Why you must restart JMRI/LccPro after firmware changes
5. **GPIO integration** (Part 2): ConfiguredProducer, ConfiguredConsumer, and scaling patterns

## Part 2: Additional Hardware Requirements

When you reach Part 2 (adding physical GPIO), you'll need these additional components:

| Component | Quantity | Notes |
|-----------|----------|-------|
| Tactile pushbutton switches | 2 | Normally-open, momentary contact |
| LEDs | 2 | Any color; 5mm standard size recommended |
| 220Ω resistors | 2 | Current limiting for LEDs (5% tolerance, 1/4W) |
| Jumper wires | ~6 | Male-to-male (you already have these from Chapter 5) |

**Note:** You already have a breadboard and basic jumper wires from the CAN setup in Chapter 5.

**Budget Estimate:** $2-3 USD for the new components (or free if you have a basic electronics kit).

### Where to Order

These components are available from Amazon, eBay, AliExpress, Adafruit, SparkFun, DigiKey, Mouser, or local hobby electronics stores.

## GPIO Pin Assignments (Part 2 Preview)

For reference, Part 2 will use these GPIO pins:

| Component | GPIO Pin | Notes |
|-----------|----------|-------|
| Button 1 | GPIO 18 | Safe general-purpose pin |
| Button 2 | GPIO 21 | Safe general-purpose pin |
| LED 1 | GPIO 19 | Safe general-purpose pin |
| LED 2 | GPIO 22 | Safe general-purpose pin |

**Important:** These pins don't conflict with CAN (GPIO 4/5) or UART (GPIO 1/3).

## What's Next

Start with [Configuration Evolution](config-evolution.md) to learn how to add features without breaking existing configurations.

Once you understand the configuration patterns, you'll be ready to add physical I/O in the subsequent sections.
- Reduce noise by keeping jumper wires away from USB cable