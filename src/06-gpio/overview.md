# Physical I/O with GPIO

Now that you've successfully created a CAN-based OpenLCB node in Chapter 5, it's time to add physical hardware interactions. But before we wire up buttons and LEDs, we'll add a useful enable/disable toggle to control the async_blink event production. This serves two purposes: it reduces message clutter (both serial output and LCC traffic in JMRI/LccPro) when we're focusing on GPIO events, and it demonstrates how to implement binary (on/off) configuration options in OpenMRNLite.

## Chapter Structure

This chapter takes a strategic two-part approach:

### Part 1: Configuration Evolution (No Additional Hardware)

Before adding GPIO pins, we'll enhance the async_blink example with:
- A configurable **enable/disable feature** for event production to reduce message clutter (serial output and LCC traffic)
- Demonstrating **binary (boolean) configuration values** using mapped integers

**You can complete Part 1 using the existing CAN setup from Chapter 5**—no additional components needed.

This enhancement gives you control over the async_blink output and introduces an important configuration pattern you'll use throughout your OpenLCB nodes.

### Part 2: Adding Physical I/O (Additional Hardware Required)

Once the configuration foundation is solid, we'll add real-world interactions:
- Connect **pushbuttons** to detect physical input  
- Connect **LEDs** to provide visual output  
- Implement OpenLCB **producers and consumers** tied to GPIO pins  
- Scale to **multiple inputs and outputs** using efficient patterns  

## What You'll Learn

By the end of this chapter, you'll understand:

1. **Boolean configuration**: Implementing enable/disable toggles in OpenMRNLite's CDI system using mapped integers
2. **CDI caching behavior**: Why you must restart JMRI/LccPro after firmware changes
3. **GPIO integration** (Part 2): ConfiguredProducer, ConfiguredConsumer, and scaling patterns

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

## What's Next

Start with [Configuration Evolution](config-evolution.md) to add the enable/disable toggle and learn about binary configuration values.

Once you've added this feature, you'll be ready to add physical I/O in the subsequent sections.
- Reduce noise by keeping jumper wires away from USB cable