// FIXME: your file license if you have one

#include "Led.h"

namespace android::led_hidl::hardware::led::implementation {

// Methods from ::android::led_hidl::hardware::led::V1_0::ILed follow.
Led::Led() {
  state = LedStatus::LED_BAD_VALUE;
  ALOGE("ledImpl Init status:%d", state);
}
Return<::android::led_hidl::hardware::led::V1_0::LedStatus> Led::get() {
  ALOGE("[%s]", __FUNCION__);
  return state;
}

Return<int32_t>
Led::set(::android::led_hidl::hardware::led::V1_0::LedStatus val) {
  ALOGE("[%s]", __FUNCION__);
  return int32_t{};
}

Return<void> Led::getBrightnessRange(getBrightnessRange_cb _hidl_cb) {
  ALOGE("[%s]", __FUNCION__);
  return Void();
}

Return<bool> Led::setBrightnessValue(const hidl_vec<int32_t> &range) {
  ALOGE("[%s]", __FUNCION__);
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

ILed *HIDL_FETCH_ILed(const char * /* name */) { return new Led(); }

} // namespace android::led_hidl::hardware::led::implementation
