/*
 * Copyright (C) 2019-2021 The LineageOS Project
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

#define LOG_TAG "HighTouchPollingRateService"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <touch/sony/HighTouchPollingRate.h>
#include <cstdio>
#include <fstream>

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

#ifdef SONY_PANEL_CTRL_PATH
static constexpr const char* kPanelFrameRatePath = SONY_PANEL_CTRL_PATH "/frame_rate_np";
#else
static constexpr const char* kPanelFrameRatePath =
        "/sys/devices/virtual/input/lxs_ts_input/frame_rate_np";
#endif

#define TOUCH_RATE_PREFIX "0 "

static inline auto send_cmd(const char* cmd) {
    LOG(INFO) << "send_cmd: " << cmd << " to " << kPanelFrameRatePath;
    return android::base::WriteStringToFile(cmd, kPanelFrameRatePath);
}

Return<bool> HighTouchPollingRate::isEnabled() {
    std::string touch_str;

    int disp_mode, touch_mode;
    auto ret = android::base::ReadFileToString(kPanelFrameRatePath, &touch_str);
    auto result = sscanf(kPanelFrameRatePath, "%d,%d", &disp_mode, &touch_mode);

    return ret && result == 2 && touch_mode > 2;
}

Return<bool> HighTouchPollingRate::setEnabled(bool enabled) {
    bool result;
    if (enabled)
        result = send_cmd(TOUCH_RATE_PREFIX "3");
    else
        result = send_cmd(TOUCH_RATE_PREFIX "2");

    if (!result) {
        LOG(ERROR) << "Failed to write sec_ts cmd!";
        return false;
    }
    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
