// FIXME: your file license if you have one
#define LOG_TAG "LedService"
#include "Led.h"
#include <log/log.h>
namespace android::led_hidl::hardware::led::implementation {

// Methods from ::android::led_hidl::hardware::led::V1_0::ILed follow.
Led::Led() {
  state = LedStatus::LED_BAD_VALUE;
  ALOGE("ledImpl Init status:%d", state);
}
Return<::android::led_hidl::hardware::led::V1_0::LedStatus> Led::get() {
  ALOGE("[%s]", __func__);
  return state;
}

Return<int32_t>
Led::set(::android::led_hidl::hardware::led::V1_0::LedStatus val) {
  state = val;
  ALOGE("[%s]", __func__);
  return int32_t{};
}

Return<void> Led::getBrightnessRange(getBrightnessRange_cb _hidl_cb) {
  ALOGE("[%s]", __func__);
  BrightnessRange range;
  range.max = 100;
  range.min = 1;
  _hidl_cb(true, range);
  return Void();
}

Return<bool> Led::setBrightnessValue(const hidl_vec<int32_t> &range) {
  ALOGE("[%s]", __func__);
  auto iter = range.begin();
  ALOGE(" getBrightnessValue range.begin: %d", *iter);
  iter = range.end();
  ALOGE(" getBrightnessValue range.end: %d", *iter);
  ALOGE(" getBrightnessValue range.size: %zu", range.size());
  return bool{};
}

Return<void> Led::on() {
  state = V1_0::LedStatus::LED_ON;
  ALOGE("Led on status:%d", state);
  return Void();
}

Return<void> Led::off() {
  state = V1_0::LedStatus::LED_OFF;
  ALOGE("Led off status:%d", state);
  return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

// ILed *HIDL_FETCH_ILed(const char * /* name */) { return new Led(); }

} // namespace android::led_hidl::hardware::led::implementation
