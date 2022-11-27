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