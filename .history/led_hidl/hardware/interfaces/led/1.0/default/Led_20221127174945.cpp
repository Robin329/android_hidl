// FIXME: your file license if you have one

#include "Led.h"

namespace android::hardware::led::implementation {

// Methods from ::android::hardware::led::V1_0::ILed follow.
Return<::android::hardware::led::V1_0::LedStatus> Led::get() {
    // TODO implement
    return ::android::hardware::led::V1_0::LedStatus {};
}

Return<int32_t> Led::set(::android::hardware::led::V1_0::LedStatus val) {
    // TODO implement
    return int32_t {};
}

Return<void> Led::getBrightnessRange(getBrightnessRange_cb _hidl_cb) {
    // TODO implement
    return Void();
}

Return<bool> Led::setBrightnessValue(const hidl_vec<int32_t>& range) {
    // TODO implement
    return bool {};
}

Return<void> Led::on() {
    // TODO implement
    return Void();
}

Return<void> Led::off() {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//ILed* HIDL_FETCH_ILed(const char* /* name */) {
    //return new Led();
//}
//
}  // namespace android::hardware::led::implementation
