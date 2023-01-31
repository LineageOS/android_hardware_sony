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

static constexpr const char* kPanelCmdPath = "/sys/devices/virtual/sec/tsp/cmd";
static constexpr const char* kPanelCmdResultPath = "/sys/devices/virtual/sec/tsp/cmd_result";

#define SET_STAMINA_CMD(status) "stamina_enable," status
#define GET_STAMINA_CMD "get_stamina_mode"

#define SET_REPORT_RATE_CMD(status) "doze_mode_change," status
#define GET_REPORT_RATE_CMD "get_doze_mode"

static inline bool send_cmd(const char *cmd) {
    return android::base::WriteStringToFile(cmd, kPanelCmdPath);
}

static inline bool send_cmd_get_result(const char* cmd, std::string& result) {
    return send_cmd(cmd) && android::base::ReadFileToString(kPanelCmdResultPath, &result);
}

Return<bool> HighTouchPollingRate::isEnabled() {
    bool ret;
    int result;
    std::string i;

    int stamina_mode = 0;
    ret = send_cmd_get_result(GET_STAMINA_CMD, i);
    result = sscanf(i.c_str(), "get_stamina_mode:%d", &stamina_mode);
    if (!ret || result < 0) return false;

    int doze_mode = 0;
    int rate_mode = 0;
    ret = send_cmd_get_result(GET_REPORT_RATE_CMD, i);
    result = sscanf(i.c_str(), "get_doze_mode:%d,%d", &doze_mode, &rate_mode);

    LOG(INFO) << "Got stamina_mode: " << stamina_mode << " doze_mode: " << doze_mode
              << " rate_mode: " << rate_mode;
    return ret && result > 0 && stamina_mode == 0 && rate_mode > 0;
}

Return<bool> HighTouchPollingRate::setEnabled(bool enabled) {
    std::string device_name = android::base::GetProperty("ro.product.system.device", "unknown");
    bool is_low_rate_device = device_name == "pdx203";
    LOG(INFO) << "Current device is " << device_name << " low_rate: " << is_low_rate_device;

    bool result;
    if (is_low_rate_device) {
        if (enabled)
            result = send_cmd(SET_STAMINA_CMD("0"));
        else
            result = send_cmd(SET_STAMINA_CMD("1"));
    } else {
        result = send_cmd(SET_STAMINA_CMD("0"));
        if (enabled)
            result &= send_cmd(SET_REPORT_RATE_CMD("2"));
        else
            result &= send_cmd(SET_REPORT_RATE_CMD("1"));
    }

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
