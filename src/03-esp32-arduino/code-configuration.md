## Code Configuration

Now we'll create the actual OpenLCB node that produces alternating events. This demonstrates the core OpenLCB protocol behavior you learned in Chapter 1 (node initialization and event production) without requiring physical hardware.

### Creating the Configuration Header

OpenMRNLite requires a configuration structure (CDI - Configuration Description Information) even for simple nodes. We'll create a minimal config.h file.

Create the file `include/config.h` with this content:

```cpp
#ifndef _ASYNC_BLINK_CONFIG_H_
#define _ASYNC_BLINK_CONFIG_H_

#include "openlcb/ConfigRepresentation.hxx"
#include "openlcb/MemoryConfig.hxx"

namespace openlcb {

/// SNIP Static Data - Manufacturer information (read-only, compiled into firmware)
extern const SimpleNodeStaticValues SNIP_STATIC_DATA = {
    4,               // Version
    "OpenMRN",       // Manufacturer
    "async_blink",   // Model
    "ESP32",         // Hardware version
    "1.00"           // Software version
};

/// SNIP Dynamic Data - User-editable node name and description
/// These are stored in the config file and can be read/written via JMRI
static const char SNIP_NODE_NAME[] = "async_blink";
static const char SNIP_NODE_DESC[] = "ESP32 Blink demo";

/// Version number for the configuration structure
static constexpr uint16_t CANONICAL_VERSION = 0x0001;

/// Minimal configuration segment with just internal config
CDI_GROUP(AsyncBlinkSegment, Segment(MemoryConfigDefs::SPACE_CONFIG), Offset(128));
CDI_GROUP_ENTRY(internal_config, InternalConfigData);
CDI_GROUP_END();

/// The main CDI structure
CDI_GROUP(ConfigDef, MainCdi());
CDI_GROUP_ENTRY(ident, Identification);
CDI_GROUP_ENTRY(acdi, Acdi);
CDI_GROUP_ENTRY(userinfo, UserInfoSegment, Name("User Info"));
CDI_GROUP_ENTRY(seg, AsyncBlinkSegment, Name("Settings"));
CDI_GROUP_END();

} // namespace openlcb

#endif // _ASYNC_BLINK_CONFIG_H_
```

**What this does**: Defines the CDI (Configuration Description Information) structure that OpenMRNLite uses to expose node configuration to JMRI. The configuration includes:
- **SNIP Static Data**: Read-only manufacturer, model, and version information (compiled into firmware)
- **SNIP Dynamic Data**: User-editable node name and description stored in the config file (visible in JMRI node properties)
- **Acdi and UserInfo**: Standard OpenLCB configuration segments
- **AsyncBlinkSegment**: Internal configuration area for this node (currently minimal, but available for future expansion)

> **Configuration Storage**: Configuration is saved to SPIFFS on first boot (via `factory_reset()` shown in the next section) and persists across restarts. In v0.1, the initial configuration is hardcoded in the constants above. See the next chapter "Configuration & Persistence" (Chapter 4) to understand how configuration storage works, how to edit settings via JMRI, and how `apply_configuration()` fits into the lifecycle.
