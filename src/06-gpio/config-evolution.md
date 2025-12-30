# Configuration Evolution with Reserved Space

In this section, we'll demonstrate a critical pattern for evolving OpenLCB node configurations without forcing users to lose their settings: **reserved space allocation**.

## The Problem: Configuration Version Changes

When you modify your node's CDI (Configuration Description Information) structure, OpenMRNLite checks whether the stored configuration version matches the firmware's expected version:

```cpp
static constexpr uint16_t CANONICAL_VERSION = 0x0003;
```

If the versions don't match, OpenMRNLite triggers a **complete factory reset**, wiping all user configuration including:
- Node name and description (ACDI data)
- Custom event IDs
- Application-specific settings
- Everything except the first 128 bytes

This happens because OpenMRNLite cannot migrate configuration fields between versions—it's an all-or-nothing reset.

## The Solution: Reserve Space Up Front

By adding reserved bytes to your configuration structure **before you need them**, you can add new fields later without changing the total configuration size, avoiding the version mismatch that triggers factory resets.

### Step 1: Add Reserved Space and Enable/Disable Feature

Let's enhance our async_blink example with:
1. A boolean "Blink Enabled" setting to enable/disable event production
2. 32 bytes of reserved space for future GPIO configuration

**Important:** Because we're adding new fields, we must increment `CANONICAL_VERSION` from `0x0002` to `0x0003`. This will trigger one final factory reset when users first flash this version, but all future additions that consume the reserved space won't require a version change.

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
+CDI_GROUP_ENTRY(reserved, BytesConfigEntry<32>,
+                Name("Reserved"),
+                Description("Reserved space for future configuration expansion"));
 CDI_GROUP_END();
```

**Key Points:**
- `BytesConfigEntry<32>` allocates 32 bytes of reserved space
- `CANONICAL_VERSION` incremented to `0x0003` (this will trigger one final factory reset)
- `blink_enabled` uses `Uint8ConfigEntry` with `MapValues` to provide "Enabled/Disabled" labels in JMRI
- OpenMRNLite doesn't have a dedicated boolean type, so we use a mapped integer (0=Disabled, 1=Enabled)
- JMRI will render this as a dropdown with "Enabled" and "Disabled" options

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
   - The "Reserved" field won't be visible in the UI (intentionally hidden)

3. **Test the enable/disable feature**:
   - Set "Blink Enabled" to "Disabled"
   - Click "Write" or "Save Changes" to write the new value to the node
   - **Important:** Click "More..." then "Update Complete" to signal the node to apply changes
   - Watch the serial console: `Configuration updated: blink_enabled = Disabled, ...`
   - Events should stop being produced
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
| 132-163 | reserved | 32 bytes | Reserved for future GPIO configuration |
| **Total** | | **~164 bytes** | |

## Future Expansion: Consuming Reserved Space

When we add GPIO configuration in the next section, we'll:

1. **Reduce reserved space** from 32 bytes to a smaller value
2. **Add new fields** that consume the difference
3. **Keep `CANONICAL_VERSION` unchanged** (critical!)

Because the total configuration size won't change, OpenMRNLite won't trigger a factory reset, and users will keep their existing settings.

For example, if we add a `ProducerConfig` (approximately 12 bytes):

```cpp
CDI_GROUP_ENTRY(button1, ProducerConfig, Name("Button 1"));
CDI_GROUP_ENTRY(reserved, BytesConfigEntry<20>,  // Was 32, now 20
                Name("Reserved"),
                Description("Reserved space for future configuration expansion"));
```

Total size remains 164 bytes, so `CANONICAL_VERSION` stays at `0x0003`, and no factory reset occurs.

## Key Takeaways

1. **Reserve space early**: Add extra bytes before you need them
2. **Consume reserved space gradually**: Shrink the reserved field as you add new configuration
3. **Don't increment version unnecessarily**: Only increment when the total size changes
4. **Restart tools after CDI changes**: JMRI/LccPro won't auto-reload CDI
5. **Use meaningful labels**: "Enabled/Disabled" is clearer than "Yes/No" for feature toggles

This pattern enables smooth configuration evolution without frustrating users with lost settings.

**Next:** [Adding Physical I/O: Button and LED](single-button-led.md)
