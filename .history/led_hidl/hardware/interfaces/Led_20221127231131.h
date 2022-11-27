// FIXME: your file license if you have one

#pragma once

#include <android/led_hidl/hardware/led/1.0/ILed.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android::led_hidl::hardware::led::implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

struct Led : public V1_0::ILed {
  // Methods from ::android::hardware::led::V1_0::ILed follow.
public:
  Led();
  Return<::android::led_hidl::hardware::led::V1_0::LedStatus> get() override;
  Return<int32_t>
  set(::android::led_hidl::hardware::led::V1_0::LedStatus val) override;
  Return<void> getBrightnessRange(getBrightnessRange_cb _hidl_cb) override;
  Return<bool> setBrightnessValue(const hidl_vec<int32_t> &range) override;
  Return<void> on() override;
  Return<void> off() override;

  // Methods from ::android::hidl::base::V1_0::IBase follow.
private:
  LedStatus state;
};

// FIXME: most likely delete, this is only for passthrough implementations
extern "C" ILed *HIDL_FETCH_ILed(const char *name);

} // namespace android::led_hidl::hardware::led::implementation
