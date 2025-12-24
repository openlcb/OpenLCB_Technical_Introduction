## Building and Uploading to ESP32

Now that you have the complete code, let's build it, upload it to your ESP32, and verify it works through the serial monitor.

### Building the Project

1. **Save the modified main.cpp file** (`Ctrl+S` / `Cmd+S`)

2. **Open the PlatformIO sidebar**:
   - Click the PlatformIO icon (alien head) in VS Code's left sidebar
   - Or use the bottom toolbar's checkmark icon (Build)

3. **Build the project**:
   - In PROJECT TASKS → **esp32doit-devkit-v1**, click **Build**
   - Or click the checkmark (✓) icon in the bottom toolbar
   - Or press `Ctrl+Alt+B` / `Cmd+Alt+B`

4. **Watch the build output**:
   ```
   Building in release mode
   Compiling .pio/build/esp32doit-devkit-v1/src/main.cpp.o
   Linking .pio/build/esp32doit-devkit-v1/firmware.elf
   Building .pio/build/esp32doit-devkit-v1/firmware.bin
   ========================= [SUCCESS] Took 5.23 seconds =========================
   ```

The first build takes longer because it compiles OpenMRNLite. Subsequent builds are much faster.

**If the build fails**, check:
- WiFi credentials are properly quoted (strings)
- All braces `{}` and parentheses `()` match
- `#include <OpenMRNLite.h>` is present
- `lib_deps` in `platformio.ini` includes OpenMRNLite

### Connecting Your ESP32

1. **Connect the ESP32 to your computer** via USB cable
   - Use a **data cable**, not a charge-only cable
   - The ESP32 should power on (onboard LED may light up)

2. **Identify the COM port** (Windows) or device path (Mac/Linux):
   - PlatformIO usually auto-detects the port
   - Windows: `COM3`, `COM4`, etc.
   - Mac: `/dev/cu.usbserial-*` or `/dev/cu.wchusbserial*`
   - Linux: `/dev/ttyUSB0` or `/dev/ttyACM0`

3. **If the port isn't detected**, you may need to install a USB driver:
   - **CP2102**: Download from [Silicon Labs](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
   - **CH340**: Download from [WCH](http://www.wch-ic.com/downloads/CH341SER_EXE.html)
   - Most modern operating systems include these drivers

### Uploading the Firmware and Opening the Monitor

1. **Upload the firmware and automatically open the serial monitor**:
   - In PROJECT TASKS → **esp32doit-devkit-v1**, click **Upload and Monitor**
   - Or press `Ctrl+Alt+U` then `Ctrl+Alt+S` / `Cmd+Alt+U` then `Cmd+Alt+S`

2. **Watch the upload process**:
   ```
   Configuring upload protocol...
   Looking for upload port...
   Auto-detected: COM3
   Uploading .pio/build/esp32doit-devkit-v1/firmware.bin
   esptool.py v4.5.1
   Connecting........__
   Chip is ESP32-D0WDQ6 (revision 1)
   Writing at 0x00010000... (100 %)
   Wrote 876544 bytes (543210 compressed) at 0x00010000 in 48.2 seconds
   Leaving...
   Hard resetting via RTS pin...
   ========================= [SUCCESS] Took 52.91 seconds =========================
   ```

3. **The ESP32 will automatically reboot** and start running your code

**If upload fails**:
- **"Serial port not found"**: Check USB cable connection, try different USB port
- **"Failed to connect"**: Hold the BOOT button while clicking "Upload and Monitor", release after "Connecting..." appears
- **Permission denied (Linux)**: Add your user to the `dialout` group: `sudo usermod -a -G dialout $USER`, then log out and back in

**If you missed the startup sequence**, press the **RESET** button on your ESP32 board to restart it and the serial monitor will display the output again.

### Verifying the Output

On first run, you should see output like this:

```
=== OpenLCB async_blink ESP32 Example ===
Node ID: 0x050201020200
Event 0: 0x0502010202000000
Event 1: 0x0502010202000001

Initializing SPIFFS...
E (523) SPIFFS: mount failed, -10025
SPIFFS initialized successfully

Connecting to WiFi SSID: YourNetwork
.....
WiFi connected!
IP Address: 192.168.1.100

Creating CDI configuration descriptor...
[CDI] Checking /spiffs/cdi.xml...
[CDI] File /spiffs/cdi.xml does not exist
[CDI] Updating /spiffs/cdi.xml (len 987)
[CDI] Registering CDI with stack...
Initializing OpenLCB configuration...
Creating config file /spiffs/openlcb_config

Starting OpenLCB stack...
Starting executor thread...
Starting TCP Hub on port 12021...
TCP Hub listening. JMRI can connect to this device on port 12021

OpenLCB node initialization complete!
Entering run mode - will alternate events every 1 second

Produced event: 0x0502010202000000 (state: 0)
Produced event: 0x0502010202000001 (state: 1)
Produced event: 0x0502010202000000 (state: 0)
Produced event: 0x0502010202000001 (state: 1)
...
```

**First Run Note**: The initial startup (especially the "Creating CDI configuration" and "Creating config file" steps) takes 20-30 seconds due to SPIFFS filesystem formatting. The SPIFFS mount error is normal—the filesystem doesn't exist yet, but it's automatically created. Don't interrupt the ESP32 during this time. Subsequent boots skip formatting and start much faster.

**What to verify**:
- ✅ SPIFFS initializes successfully
- ✅ WiFi connects successfully (shows your network name and IP address)
- ✅ OpenLCB stack initializes
- ✅ Events alternate between `...00` and `...01` every second
- ✅ State toggles between `0` and `1`

### Understanding What You See

This output confirms your ESP32 is:

1. **Connected to WiFi**: The IP address shows it's on your network
2. **Running the OpenLCB stack**: Node initialization completed successfully
3. **Producing events**: The alternating event IDs prove the event production logic works
4. **Ready for network communication**: The node is broadcasting these events on the OpenLCB network (you'll verify this with JMRI next)

**Congratulations!** You have a working OpenLCB node. The events are being broadcast over WiFi/TCP, but you can't see them on the network yet - that's what JMRI will show you in the next section.

### Common Issues

**WiFi won't connect** (stuck on dots):
- Verify WiFi credentials in code are correct
- Check ESP32 is within range of your access point
- Confirm your network is 2.4GHz (not 5GHz only)
- Some corporate/school networks block device connections

**No serial output at all**:
- Verify `monitor_speed = 115200` is in your `platformio.ini` file
- Try pressing RESET button on ESP32
- Verify the serial monitor is connected to the correct port

**Output is garbled/random characters**:
- Wrong baud rate - ensure `monitor_speed = 115200` is in `platformio.ini`
- If you already added it, stop and restart the serial monitor
- Bad USB cable or connection

**"Brownout detector triggered" errors**:
- Insufficient power from USB port
- Try a different USB port or powered USB hub
- This usually doesn't prevent operation, just a warning
