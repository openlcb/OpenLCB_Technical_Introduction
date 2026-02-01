# Adding an Enable/Disable Toggle

Let's add a "Blink Enabled" toggle to the async_blink example. This demonstrates an important OpenMRNLite pattern: **binary configuration values using mapped integers**.

## Implementing Binary Configuration Values

### Step 1: Add the Enable/Disable Feature

Let's add a "Blink Enabled" toggle to control whether the async_blink node produces events. This will quiet down both the serial monitor and the JMRI/LccPro event monitor, making it easier to focus on GPIO events when we add them in the next section.

This also demonstrates an important OpenMRNLite pattern: **binary configuration values using mapped integers**. Since OpenMRNLite doesn't have a dedicated boolean type, we'll use a `Uint8ConfigEntry` with a `MapValues` attribute that maps 0→"Disabled" and 1→"Enabled" for user-friendly display in JMRI.

**Note:** Because we're adding a new field to the configuration structure, we must increment `CANONICAL_VERSION` from `0x0002` to `0x0003`. As covered in Chapter 4, this will trigger a factory reset when you first flash this version.

#### Updated config.h

We'll add these changes to our CDI:

```diff
 namespace openlcb {
 
 // ... SNIP static/dynamic data unchanged ...
 
+/// Boolean map for Enabled/Disabled configuration values
+static constexpr const char *BOOLEAN_MAP =
+    "<relation><property>0</property><value>Disabled</value></relation>"
+    "<relation><property>1</property><value>Enabled</value></relation>";
+
 /// Version number for the configuration structure
-static constexpr uint16_t CANONICAL_VERSION = 0x0002;
+static constexpr uint16_t CANONICAL_VERSION = 0x0003;
 
 /// Minimal configuration segment with application settings
 CDI_GROUP(AsyncBlinkSegment, Segment(MemoryConfigDefs::SPACE_CONFIG), Offset(128));
 CDI_GROUP_ENTRY(internal_config, InternalConfigData);
+CDI_GROUP_ENTRY(blink_enabled, Uint8ConfigEntry,
+                Default(1),
+                Min(0), Max(1),
+                MapValues(BOOLEAN_MAP),
+                Name("Blink Enabled"),
+                Description("Enable or disable event production"));
 CDI_GROUP_ENTRY(blink_interval, Uint16ConfigEntry,
                 Default(1000),
                 Min(100),
                 Max(30000),
                 Name("Blink Interval"),
                 Description("Milliseconds between alternating events (100-30000)"));
 CDI_GROUP_END();
```

**Key Points:**
- **Mapped integers for binary values**: OpenMRNLite doesn't have a dedicated boolean type, so we use `Uint8ConfigEntry` with `MapValues` to create user-friendly "Enabled/Disabled" labels
- The mapping: `0` displays as "Disabled", `1` displays as "Enabled" in JMRI
- `CANONICAL_VERSION` incremented to `0x0003` (triggers factory reset as explained in Chapter 4)
- JMRI will render this as a dropdown with "Enabled" and "Disabled" options instead of raw numbers

#### Updated main.cpp

Now we need to read and apply the new `blink_enabled` configuration:

```diff
 // State variable to track which event to send
 bool event_state = false;
 
-// Timing for event production - cached from configuration
+// Configuration variables - cached from configuration
+bool blink_enabled = true;           // Default enabled, will be read from config in setup()
 unsigned long last_event_time = 0;
 unsigned long event_interval = 100;  // Default, will be read from config in setup()
```

Update `apply_configuration()` to read both settings:

```diff
     UpdateAction apply_configuration(int fd, bool initial_load,
                                      BarrierNotifiable *done) OVERRIDE
     {
         AutoNotify n(done);
         
-        // Read the blink interval from config file and update global variable
+        // Read configuration values from config file and update global variables
+        blink_enabled = cfg.seg().blink_enabled().read(fd);
         event_interval = cfg.seg().blink_interval().read(fd);
-        Serial.printf("Configuration updated: blink_interval = %lu ms\n", event_interval);
+        Serial.printf("Configuration updated: blink_enabled = %s, blink_interval = %lu ms\n",
+                      blink_enabled ? "Enabled" : "Disabled", event_interval);
         
         return UPDATED;
     }
```

Update `factory_reset()` to initialize the new field:

```diff
     void factory_reset(int fd) override
     {
         // Initialize SNIP dynamic data on first boot
         cfg.userinfo().name().write(fd, openlcb::SNIP_NODE_NAME);
         cfg.userinfo().description().write(fd, openlcb::SNIP_NODE_DESC);
         
         // Initialize application settings with defaults
+        cfg.seg().blink_enabled().write(fd, 1);      // Default enabled
         cfg.seg().blink_interval().write(fd, 1000);  // Default 1 second
-        Serial.println("Factory reset: wrote default blink_interval = 1000 ms");
+        Serial.println("Factory reset: wrote defaults (blink_enabled=Enabled, blink_interval=1000 ms)");
     }
```

Update `loop()` to check the enabled flag before producing events:

```diff
 void loop() {
   // CRITICAL: Must call openmrn.loop() frequently to process messages
   openmrn.loop();
   
-  // Check if it's time to produce an event
-  // event_interval is read from config in setup() and updated in apply_configuration()
+  // Check if blinking is enabled and if it's time to produce an event
+  // blink_enabled and event_interval are read from config and updated in apply_configuration()
   unsigned long current_time = millis();
   
-  if (current_time - last_event_time >= event_interval) {
+  if (blink_enabled && current_time - last_event_time >= event_interval) {
     // Alternate event state
     event_state = !event_state;
     
     // Send the event
     // ... rest of event production code unchanged ...
   }
 }
```

## Complete Working Code

> The complete working code at this point is available on GitHub:
>
> - **PlatformIO version:** [code/platformio/chapter6/v1_toggle/](https://github.com/openlcb/OpenLCB_Technical_Introduction/tree/master/code/platformio/chapter6/v1_toggle/)
> - **Arduino IDE version:** [code/arduino/chapter6/v1_toggle/](https://github.com/openlcb/OpenLCB_Technical_Introduction/tree/master/code/arduino/chapter6/v1_toggle/)
>
> You can download or clone the repository and open either version to get the exact code with the enable/disable toggle implemented.

### Step 2: Test the Configuration

After building and uploading the firmware:

1. **Observe the factory reset** (due to version increment):
   ```
   Factory reset: wrote defaults (blink_enabled=Enabled, blink_interval=1000 ms)
   ```

2. **Configure via JMRI/LccPro**:
   - Open the node's configuration dialog
   - You should see "Blink Enabled" with a dropdown showing "Enabled" / "Disabled"
   - You should see "Blink Interval" with the existing 100-30000 ms range

3. **Test the enable/disable feature**:
   - Set "Blink Enabled" to "Disabled"
   - Click "Write" or "Save Changes" to write the new value to the node
   - **Important:** Click "More..." then "Update Complete" to signal the node to apply changes
   - Watch the serial console: `Configuration updated: blink_enabled = Disabled, ...`
   - **Both event production and serial output stop** — notice how much quieter the output becomes
   - Also check JMRI/LccPro's Event Monitor — no more async_blink events cluttering the view
   - Re-enable and verify events resume (remember to click "Update Complete" again)

## Important: CDI Caching in JMRI/LccPro

**⚠️ Critical Limitation:** JMRI and LccPro aggressively cache CDI data. If you change your firmware's CDI structure and reflash, **you must restart JMRI/LccPro** to see the updated configuration fields.

### Why CDI is Cached

- **CDI is expensive to fetch**: It's retrieved via streamed datagrams and can be tens of kilobytes
- **Tools assume CDI is static**: The LCC spec treats CDI as a description of the node that rarely changes
- **No protocol for CDI change notification**: There's no way for a node to tell configuration tools "my CDI changed, please reload"

### When CDI is Refreshed

Both JMRI and LccPro only reload CDI when:
- The tool is restarted
- The node is removed from the roster and re-added (JMRI only)

**Tip:** LccPro is faster to restart than JMRI, so many developers use it during rapid CDI iteration.

## Memory Layout After Changes

Our configuration now looks like this:

| Offset | Field | Size | Purpose |
|--------|-------|------|---------|
| 0-127 | ACDI User Data | 128 bytes | Node name, description |
| 128 | InternalConfigData | Variable | Version tracking, etc. |
| 129 | blink_enabled | 1 byte | Enable/disable event production |
| 130-131 | blink_interval | 2 bytes | Milliseconds between events |
| **Total** | | **~132 bytes** | |

## Key Takeaways

1. **Mapped integers for binary values**: Use `Uint8ConfigEntry` with `MapValues` to create user-friendly on/off toggles
2. **Value mapping syntax**: `<relation><property>VALUE</property><value>LABEL</value></relation>`
3. **Restart tools after CDI changes**: JMRI/LccPro won't auto-reload CDI
4. **Use meaningful labels**: "Enabled/Disabled" is clearer than numeric values
5. **Practical benefit**: The enable/disable toggle keeps message monitors clean when focusing on specific events

**Next:** [Adding Physical I/O: Button and LED](single-button-led.md)
