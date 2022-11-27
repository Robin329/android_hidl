#define LOG_TAG "android.hardware.led@1.0-service"

#include "Led.h"
#include <android/hardware/led/1.0/ILed.h>
#include <hidl/LegacySupport.h>

using ::android::OK;
using android::sp;
using ::android::status_t;

// libhwbinder:
using ::android::hardware::configureRpcThreadpool;
using android::hardware::defaultPassthroughServiceImplementation;
using ::android::hardware::joinRpcThreadpool;
using android::hardware::led::V1_0::ILed;
using android::hardware::led::V1_0::implementation::Led;

int main(int /* argc */, char ** /* argv */) {
#if 0
// Passthrought dlopen so method
return defaultPassthroughServiceImplementation<Iled>();
#else
  status_t status;
  sp<Iled> service = new Led();
  configureRpcThreadpool(1, true /* calllerWillJoin */);
  status = service->registerAsService();
  if (android::OK != status) {
    ALOGE("Could not register service for led(%d)", status);
    return 1;
  }
  ALOGI("Led service is ready");
  joinRpcThreadpool();
  // In normal operation, we don't expect the thread pool to exit
  ALOGE("Led service is shutting down");
#endif
  return 0;
}