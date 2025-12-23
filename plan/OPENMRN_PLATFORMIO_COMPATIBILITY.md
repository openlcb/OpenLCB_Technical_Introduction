# OpenMRNLite PlatformIO Compatibility Fixes

## Problem

OpenMRNLite v2.2.1 doesn't compile in PlatformIO due to:
1. PlatformIO espressif32@6.4.0 uses Arduino-ESP32 v2.0.11 (ESP-IDF 4.4.x)
2. OpenMRNLite v2.2.1 requires Arduino-ESP32 v3.3.x (ESP-IDF 5.1.x)
3. PlatformIO's toolchain defaults to C++11, library uses C++14

## Proposed Solution

Maintain compatibility with both ESP-IDF 4.x and 5.x using conditional compilation, allowing OpenMRNLite to work in both Arduino IDE (latest) and PlatformIO (lagging). Enable C++14 support in PlatformIO builds.

## API Compatibility Fixes

### 1. C++14 Lambda Syntax
**Files**: `Esp32HardwareI2C.cxx` (openmrn), `Esp32HardwareI2C.cpp` (OpenMRNLite)

Change lines 272, 321, 426, 445:
```cpp
// Before (C++14)
const auto &device

// After (C++11 compatible, or enable C++14 in build)
const i2c_device_t &device
```

### 2. FreeRTOS API
**File**: `OpenMRNLite/src/OpenMRNLite.h` line 413

```cpp
// Before (ESP-IDF 5.x only)
xTaskGetIdleTaskHandleForCore(current_core)

// After (works in both 4.x and 5.x)
xTaskGetIdleTaskHandleForCPU(current_core)
```

### 3. Watchdog Timer API
**File**: `OpenMRNLite/src/OpenMRNLite.h` lines 438-451

```cpp
// Before (ESP-IDF 5.x only)
esp_task_wdt_config_t twdt_config = {...};
ESP_ERROR_CHECK(esp_task_wdt_reconfigure(&twdt_config));

// After (ESP-IDF 4.x compatible)
TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
ESP_ERROR_CHECK(esp_task_wdt_delete(idle_0));
```

### 4. ADC One-Shot API
**Files**: `Esp32AdcOneShot.hxx`, `Esp32AdcUnitManager.cpp`

Wrap entire ADC oneshot implementation in conditional compilation:
```cpp
#if __has_include(<esp_adc/adc_oneshot.h>)
// ADC oneshot code (ESP-IDF 5.x)
#endif
```

**Note**: ADC functionality disabled on ESP-IDF 4.x, could implement fallback using legacy ADC APIs if needed.

## Files Modified in Local Workaround

1. **openmrn/src/freertos_drivers/esp32/Esp32HardwareI2C.cxx**
   - Lambda parameter types (4 locations)

2. **OpenMRNLite/src/freertos_drivers/esp32/Esp32HardwareI2C.cpp**
   - Lambda parameter types (4 locations)

3. **OpenMRNLite/src/OpenMRNLite.h**
   - FreeRTOS API name (1 location)
   - WDT API calls (1 block)

4. **OpenMRNLite/src/freertos_drivers/esp32/Esp32AdcOneShot.hxx**
   - Conditional compilation guards (entire class)

5. **OpenMRNLite/src/freertos_drivers/esp32/Esp32AdcUnitManager.cpp**
   - Conditional compilation guards (entire implementation)

## Request for Feedback

We would appreciate guidance on:
1. Is maintaining PlatformIO compatibility a priority?
2. Which fix approach aligns best with project goals?
3. Would a pull request with conditional compilation be acceptable?
4. Any concerns about the applied workarounds?

Our goal is to use the latest OpenMRN features in PlatformIO-based ESP32 projects while respecting the library's architecture and maintenance philosophy.

---

**Contact**: [Your contact info here]
**Date**: December 19, 2025
**OpenMRNLite Version**: v2.2.1
**PlatformIO Platform**: espressif32@6.4.0
Build Configuration for PlatformIO

To enable C++14 and avoid lambda syntax changes, add to `platformio.ini`:
```ini
build_unflags = -std=gnu++11
build_flags = -std=gnu++14
```

## Testing Status

✅ Successfully builds and links in PlatformIO espressif32@6.4.0  
✅ All core OpenMRNLite functionality intact  
⚠️ ADC functionality conditionally disabled on ESP-IDF 4.x

## Next Steps

We'd like to submit a PR with these compatibility fixes. All changes use conditional compilation to maintain full functionality on ESP-IDF 5.x (Arduino IDE) while enabling builds on ESP-IDF 4.x (PlatformIO).

**Prepared by**: [Your name/contact]  
**Date**: December 19, 2025