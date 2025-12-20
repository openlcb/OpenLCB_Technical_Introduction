# Physical I/O with GPIO

Now that you've successfully created a WiFi-based OpenLCB node in Chapter 3, let's add physical hardware interactions. In this chapter, we'll connect a pushbutton and LED to your ESP32 using GPIO pins, and modify the code to respond to real-world events.

## Overview

Chapter 3 demonstrated OpenLCB event production and consumption entirely in software—alternating between two events on a 1-second cycle. Your JMRI interface saw the events being transmitted over the network, but there were no physical inputs or outputs.

In this chapter, we'll extend that example by:
- Connecting a pushbutton to a GPIO pin (input)
- Connecting an LED to a GPIO pin (output)
- Modifying the code to produce an event when the button is pressed
- Modifying the code to consume events and toggle the LED

The button and LED will interact through the OpenLCB network, allowing them to be controlled by JMRI or other nodes on the network.

## Bill of Materials

For this chapter, you'll need the following components (all commonly available):

| Component | Quantity | Notes |
|-----------|----------|-------|
| Solderless breadboard | 1 | Standard 830-hole breadboard |
| Tactile pushbutton switch | 1 | Normally-open, momentary contact |
| LED | 1 | Any color; 5mm standard size recommended |
| 220Ω resistor | 1 | Current limiting resistor for LED (5% tolerance, 1/4W) |
| Jumper wires | ~10 | Male-to-male jumper wires for breadboard connections |

**Optional (for easier debugging)**:
- Breadboard power supply with USB adapter
- Wire strippers (if making custom jumper wires)

### Where to Order

These components are available from:
- **Amazon / eBay**: Search individual component names
- **AliExpress / Banggood**: Direct from manufacturers (slower shipping, very cheap)
- **Adafruit / SparkFun**: Higher quality, better documentation, higher cost
- **DigiKey / Mouser**: Professional bulk suppliers
- **Hobby Electronics Stores**: Local retailers in your area

**Budget Estimate**: $2-5 USD for all components (or free if you have a "maker kit" lying around)

## GPIO Pin Assignment

For the ESP32 DevKit V1, we'll use the following GPIO pins:

| Component | GPIO Pin | Purpose | Notes |
|-----------|----------|---------|-------|
| Pushbutton | GPIO 12 | Input | Pulled HIGH with internal pull-up; button pulls to GND |
| LED | GPIO 13 | Output | Drives LED anode via 220Ω resistor |

These pins are chosen because:
- They're far from WiFi/USB pins (reduces interference)
- They're accessible on the standard DevKit V1 header
- GPIO 12 is not used for flash boot configuration (it must be HIGH during boot, which works fine with our pull-up strategy)
- GPIO 13 is a safe, commonly-used output pin with no special boot-mode requirements

### Alternative Pin Assignments

If you need to use different pins, you can adapt by:
- Choosing any GPIO pin **except**: GPIO 0, 2, 4, 5, 12*, 13, 14, 15, 16, 17, 25, 26, 27 (these have boot/flash mode constraints)
- **GPIO 1, 3**: Reserved for UART (serial communication)
- **GPIO 6-11**: Used for flash memory
- For maximum safety, choose from: **GPIO 18, 19, 21, 22, 23, 32, 33, 34, 35, 36, 37, 38, 39**

*GPIO 12 works but must be HIGH at boot. Our design respects this.

## Breadboard Wiring Diagram

### Button Circuit

The button is wired to GPIO 12 with an internal pull-up resistor. When the button is pressed, GPIO 12 is pulled to GND (logic 0). When released, the internal pull-up resistor brings GPIO 12 back to HIGH (logic 1).

**Wiring for the Button:**

```
ESP32 Pin GPIO 12 ----+---- Button ----+---- GND
                       |                |
                      (No external     (Button
                       resistor         switch)
                       needed;
                       internal
                       pull-up)
```

**Breadboard Layout for Button:**
1. Connect a jumper wire from **ESP32 GPIO 12** to a column on the breadboard (e.g., column A, row 1)
2. Connect the pushbutton with one pin in the same column (row 1) and the other pin in row 5 of column A
3. Connect a jumper wire from column A, row 5 to **GND** on the ESP32

The button acts as a switch: when pressed, it shorts GPIO 12 to GND.

### LED Circuit

The LED is wired to GPIO 13 through a 220Ω current-limiting resistor. This protects the LED from excessive current and the GPIO pin from damage.

**Wiring for the LED:**

```
ESP32 GPIO 13 ----+---- 220Ω Resistor ----+---- LED (+) ---- LED (-) ----+---- GND
                  |                         |                             |
              (From GPIO)             (Limits current)          (LED Cathode)
```

**Breadboard Layout for LED:**
1. Connect a jumper wire from **ESP32 GPIO 13** to a column on the breadboard (e.g., column B, row 10)
2. Insert the 220Ω resistor with one pin in column B, row 10 and the other pin in column B, row 12
3. Insert the LED with its **long pin (anode, +)** in column B, row 12 and **short pin (cathode, -)** in column B, row 14
4. Connect a jumper wire from column B, row 14 to **GND** on the ESP32

The resistor current-limits the LED to safe operating levels. Always use a resistor with LEDs!

### Complete Breadboard Layout

Here's the full picture of how everything connects:

```
Breadboard (top view):

     A    B    C    D    E
  +-----+-----+-----+-----+
1 | +12 |     |     |     |  ← GPIO 12 from ESP32
  |  |  |     |     |     |
5 |  ◯  |     |     |     |  ← Button (bottom pin to GND)
  |  |  |     |     |     |
8 |  |  |     |     |     |
  |  |  |     |     |     |
10|     | +13 |     |     |  ← GPIO 13 from ESP32
  |     |  |  |     |     |
12|     | [R] |     |     |  ← 220Ω Resistor (R)
  |     |  |  |     |     |
14|     | [L] |     |     |  ← LED (L)
  |     |  |  |     |     |
16| GND | GND|     |     |  ← GND from ESP32 (multiple connections)
  |  +  |  +  |     |     |
  +-----+-----+-----+-----+

Key:
  +12 = Wire from GPIO 12
  +13 = Wire from GPIO 13
  ◯   = Pushbutton switch
  [R] = 220Ω Resistor
  [L] = LED (long pin up, short pin down)
  GND = Wire to GND
  +   = GND bus
```

### Connection Checklist

Before applying power, verify:
- [ ] Button is connected between GPIO 12 and GND
- [ ] 220Ω resistor is in series with the LED
- [ ] LED long pin (+) is toward GPIO 13, short pin (-) is toward GND
- [ ] Both GND connections (from button and LED) go to the same GND pin on the ESP32
- [ ] No jumper wires are touching (risk of shorts)
- [ ] The breadboard power supply is **OFF** before plugging in

## Testing the Hardware

Once you've wired everything up, you can test with simple code before moving to OpenLCB events. Here's a basic Arduino sketch to verify the connections:

```cpp
// Simple GPIO test sketch
void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT_PULLUP);   // Button input with pull-up
  pinMode(13, OUTPUT);         // LED output
  digitalWrite(13, LOW);       // LED off initially
  Serial.println("GPIO test started");
}

void loop() {
  bool buttonPressed = digitalRead(12) == LOW;  // Button pulls GPIO to LOW when pressed
  
  if (buttonPressed) {
    digitalWrite(13, HIGH);  // Turn on LED
    Serial.println("Button pressed - LED ON");
  } else {
    digitalWrite(13, LOW);   // Turn off LED
    Serial.println("Button released - LED OFF");
  }
  
  delay(100);  // Debounce delay
}
```

**Expected behavior:**
- When you **press the button**, the LED should turn **on**
- When you **release the button**, the LED should turn **off**
- The serial monitor will print messages showing button state changes

If this test works, your hardware connections are correct!

## What's Next

Once you've verified the hardware is working, the next step is to integrate this with the OpenLCB code from Chapter 3:

- Modify the code to detect button presses and produce OpenLCB events
- Modify the code to consume OpenLCB events and control the LED
- Test interaction through JMRI (press the button, see the event in JMRI; send an event from JMRI, see the LED respond)

This transforms your ESP32 from a software-only node into a real-world interactive OpenLCB device!

## Troubleshooting

**LED doesn't light up:**
- Check LED orientation (long pin must be toward GPIO 13)
- Verify the resistor is not the LED (easy mistake!)
- Test with the simple GPIO sketch above to isolate the issue

**Button doesn't respond:**
- Verify button is connected between GPIO 12 and GND
- Try pressing firmly or wiggling the button wires (may be loose)
- Verify no other jumper wires are touching the button connections

**LED is very dim:**
- Resistor value might be too high
- Try a 100Ω or 150Ω resistor if available
- Some LEDs are naturally dim; try a different LED

**Random LED behavior:**
- Check for loose jumper wire connections
- Verify GND is properly connected from ESP32 to breadboard
- Reduce noise by keeping jumper wires away from USB cable