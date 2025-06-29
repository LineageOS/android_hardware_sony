/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "HighTouchPollingRateService_Lxs"

#include "HighTouchPollingRate.h"
#include "Utils.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <cstdio>
#include <fstream>

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

static constexpr const char* kPanelFrameRatePath = "/sys/devices/virtual/input/lxs_ts_input/frame_rate_np";

}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

#define TOUCH_RATE_PREFIX "0 "

ndk::ScopedAStatus HighTouchPollingRate::getEnabled(bool* _aidl_return) {
    std::string touch_str;

    int disp_mode, touch_mode;
    auto ret = android::base::ReadFileToString(kPanelFrameRatePath, &touch_str);;
    auto result = sscanf(touch_str.c_str(), "%d,%d", &disp_mode, &touch_mode);

    LOG(INFO) << __func__ << ": disp_mode: " << disp_mode << ", touch_mode: " << touch_mode;

    *_aidl_return = ret && result == 2 && touch_mode > 2;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus HighTouchPollingRate::setEnabled(bool enabled) {
    bool result;
    if (enabled)
        result = send_cmd(kPanelFrameRatePath, TOUCH_RATE_PREFIX "3");
    else
        result = send_cmd(kPanelFrameRatePath, TOUCH_RATE_PREFIX "2");

    if (!result) {
        LOG(ERROR) << "Failed to write HighTouchPollingRate state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
