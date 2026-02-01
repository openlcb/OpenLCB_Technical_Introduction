#ifndef _ASYNC_BLINK_CONFIG_H_
#define _ASYNC_BLINK_CONFIG_H_

#include "openlcb/ConfigRepresentation.hxx"
#include "openlcb/MemoryConfig.hxx"
#include "openlcb/ConfiguredProducer.hxx"
#include "openlcb/ConfiguredConsumer.hxx"

namespace openlcb {

extern const SimpleNodeStaticValues SNIP_STATIC_DATA = {
    4,               // Version
    "OpenMRN",       // Manufacturer
    "async_blink",   // Model
    "ESP32",         // Hardware version
    "1.00"           // Software version
};

static const char SNIP_NODE_NAME[] = "async_blink";
static const char SNIP_NODE_DESC[] = "ESP32 Blink demo";

/// Boolean map for Enabled/Disabled configuration values
static constexpr const char *BOOLEAN_MAP =
    "<relation><property>0</property><value>Disabled</value></relation>"
    "<relation><property>1</property><value>Enabled</value></relation>";

/// Version number for the configuration structure
static constexpr uint16_t CANONICAL_VERSION = 0x0005;

CDI_GROUP(AsyncBlinkSegment, Segment(MemoryConfigDefs::SPACE_CONFIG), Offset(128));
CDI_GROUP_ENTRY(internal_config, InternalConfigData);
CDI_GROUP_ENTRY(blink_enabled, Uint8ConfigEntry,
                Default(1),
                Min(0), Max(1),
                MapValues(BOOLEAN_MAP),
                Name("Blink Enabled"),
                Description("Enable or disable event production"));
CDI_GROUP_ENTRY(blink_interval, Uint16ConfigEntry,
                Default(1000),
                Min(100),
                Max(30000),
                Name("Blink Interval"),
                Description("Milliseconds between alternating events (100-30000)"));
CDI_GROUP_ENTRY(button,
                ProducerConfig,
                Name("Button 1"),
                Description("Physical button input on GPIO 18"));
CDI_GROUP_ENTRY(led, ConsumerConfig,
                Name("LED 1"),
                Description("LED output on GPIO 19"));
CDI_GROUP_END();

CDI_GROUP(ConfigDef, MainCdi());
CDI_GROUP_ENTRY(ident, Identification);
CDI_GROUP_ENTRY(acdi, Acdi);
CDI_GROUP_ENTRY(userinfo, UserInfoSegment, Name("User Info"));
CDI_GROUP_ENTRY(seg, AsyncBlinkSegment, Name("Settings"));
CDI_GROUP_END();

} // namespace openlcb

#endif // _ASYNC_BLINK_CONFIG_H_
