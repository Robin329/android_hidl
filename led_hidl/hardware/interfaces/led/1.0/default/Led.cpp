// FIXME: your file license if you have one
#include <cstddef>
#define LOG_TAG "LedService"
#include <log/log.h>

#include "Led.h"
namespace android::led_hidl::hardware::led::implementation {

// Methods from ::android::led_hidl::hardware::led::V1_0::ILed follow.
Led::Led(led_device_t *leds) : mLeds(std::move(leds)) {
  status = LedStatus::LED_BAD_VALUE;
  ALOGE("ledImpl Init status:%d", status);
}
Return<::android::led_hidl::hardware::led::V1_0::LedStatus> Led::get() {
  ALOGE("[%s]", __func__);
  return status;
}

Return<int32_t> Led::set(
    ::android::led_hidl::hardware::led::V1_0::LedStatus val) {
  status = val;
  ALOGE("[%s]", __func__);
  return int32_t{};
}

Return<void> Led::getBrightnessRange(getBrightnessRange_cb _hidl_cb) {
  ALOGE("[%s]", __func__);
  LedState state;
  state.maxBrightness = 100;
  state.minBrightness = 0;
  _hidl_cb(true, state);
  return Void();
}

Return<bool> Led::setBrightnessValue(const LedState &state) {
  ALOGE("[%s]", __func__);
  led_device_t *hwLed = mLeds;
  led_state_t ledState{
      .maxBrightness = state.maxBrightness,
      .minBrightness = state.minBrightness,
      .brightness = state.brightness,
  };
  int ret = hwLed->setBrightnessValue(hwLed, &ledState);
  if (ret < 0) {
    ALOGE("[%s] setBrightness failed(%d)!", __func__, ret);
    return false;
  }
  return true;
}

Return<void> Led::on() {
  status = V1_0::LedStatus::LED_ON;
  ALOGE("Led on status:%d", status);
  return Void();
}

Return<void> Led::off() {
  status = V1_0::LedStatus::LED_OFF;
  ALOGE("Led off status:%d", status);
  return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

led_device_t *getLedDevice(const char *name) {
  led_device_t *ledDevice;
  const hw_module_t *hwModule = nullptr;
  int ret = hw_get_module(LEDS_HARDWARE_MODULE_ID, &hwModule);
  if (!ret) {
    ret = hwModule->methods->open(hwModule, name,
                                  reinterpret_cast<hw_device_t **>(&ledDevice));
    if (ret != 0) {
      ALOGE("[%s] %s %s failed(%d)\n", __func__, LEDS_HARDWARE_MODULE_ID, name,
            ret);
    }
  } else {
    ALOGE("[%s] %s %s hw get module failed:%d\n", __func__,
          LEDS_HARDWARE_MODULE_ID, name, ret);
  }
  if (ret == 0) {
    return ledDevice;

  } else {
    ALOGE("[%s] led passthrough failed to load legacy HAL!!!\n", __func__);
    return nullptr;
  }
}

ILed *HIDL_FETCH_ILed(const char * /* name */) {
  const char *name = LED_ID_POWER;
  led_device_t *leds = getLedDevice(name);
  if (leds == nullptr) {
    ALOGE("[%s] get led device error!", __func__);
  }
  return new Led(std::move(leds));
}

}  // namespace android::led_hidl::hardware::led::implementation
