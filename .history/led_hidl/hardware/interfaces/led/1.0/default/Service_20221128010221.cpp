/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define LOG_TAG "android.led_hidl.hardware.led@1.0-service"

#include "Led.h"
#include <android/led_hidl/hardware/led/1.0/ILed.h>
#include <hidl/LegacySupport.h>

using ::android::OK;
using android::sp;
using ::android::status_t;

// libhwbinder:
using ::android::hardware::configureRpcThreadpool;
using android::hardware::defaultPassthroughServiceImplementation;
using ::android::hardware::joinRpcThreadpool;
using android::led_hidl::hardware::led::implementation::Led;
using android::led_hidl::hardware::led::V1_0::ILed;

int main(int /* argc */, char ** /* argv */) {
#if 1
  // Passthrought dlopen so method
  return defaultPassthroughServiceImplementation<Iled>();
#else
  status_t status;
  sp<ILed> service = new Led();
  configureRpcThreadpool(1, true /* calllerWillJoin */);
  status = service->registerAsService("Led");
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