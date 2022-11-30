
#define LOG_TAG "led_client"
#include <android/led_hidl/hardware/led/1.0/ILed.h>
#include <log/log.h>

using android::sp;
using android::hardware::hidl_vec;
using android::led_hidl::hardware::led::V1_0::ILed;
using android::led_hidl::hardware::led::V1_0::LedState;
using android::led_hidl::hardware::led::V1_0::LedStatus;

int main() {
  // BrightnessRange range;
  sp<ILed> service = ILed::getService();
  if (service == nullptr) {
    ALOGE("Can't find ILed service...");
    return -1;
  }
  ALOGE("ILed ON");
  service->on();

  ALOGE("ILed OFF");
  service->off();

  ALOGE("ILed set");
  service->set(LedStatus::LED_ON);

  ALOGE("ILed get");
  LedStatus ret = service->get();
  ALOGE("ILed get: %d", ret);

  service->getBrightnessRange([](bool ret1, LedState state) {
    ALOGE("ILed getBrightnessRange ret: %d", ret1);
    ALOGE("ILed getBrightnessRange Max: %d", state.maxBrightness);
    ALOGE("ILed getBrightnessRange Min: %d", state.minBrightness);
  });
  LedState state = {0, 0, 1};
  bool ret2 = service->setBrightnessValue(state);
  ALOGE("ILed getBrightnessValue bool: %d", ret2);
  return 0;
}
