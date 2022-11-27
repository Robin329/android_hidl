#define LOG_TAG "android.hardware.led@1.0-service"

#include "Led.h"
#include <android/hardware/led/1.0/ILed.h>
#include <hidl/LegacySupport.h>

using android::sp;
using android::hardware::configureRpcThreadpool;
using android::hardware::defaultPassthroughServiceImplementation;
using android::hardware::joinRpcThreadpool;
using android::hardware::led::V1_0::ILed;
using android::hardware::led::V1_0::implementation::Led;

int main() {
#if 0
// Passthrought dlopen so method
return defaultPassthroughServiceImplementation<Iled>();
#else
  sp<Iled> service = new Led();
  configureRpcThreadpool(1, true /* calllerWillJoin */);
  if (android::OK != service->registerAsService()) {
    ALOGE("Could not register service for led");
    return 1;
  }
  joinRpcThreadpool();
#endif
}