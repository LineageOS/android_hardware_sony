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

#include <android-base/logging.h>
#include <touch/sony/HighTouchPollingRate.h>
#include <fstream>

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

static constexpr const char* kPanelCmdPath = "/sys/devices/virtual/sec/tsp/cmd";
static constexpr const char* kPanelCmdResultPath = "/sys/devices/virtual/sec/tsp/cmd_result";

#define SET_STAMINA_CMD(status) "stamina_enable," status

#define STAMINA_ENABLE_CMD SET_STAMINA_CMD("1")
#define STAMINA_DISABLE_CMD SET_STAMINA_CMD("0")
#define STAMINA_GET_STATUS_CMD "get_stamina_mode"

Return<bool> HighTouchPollingRate::isEnabled() {
    std::ofstream file(kPanelCmdPath);
    int result = -1;
    file << STAMINA_GET_STATUS_CMD;

    std::string i;
    std::ifstream file_result(kPanelCmdResultPath);
    file_result >> i;
    for (auto c : i) {
        if (c >= '0' && c <= '9') {
            result = c - '0';
            break;
        }
    }
    LOG(DEBUG) << "Got result: " << result << " fail " << file_result.fail();
    return !file.fail() && result > 0;
}

Return<bool> HighTouchPollingRate::setEnabled(bool enabled) {
    std::ofstream file(kPanelCmdPath);

    // Enable high touch polling rate = disable stamina
    file << (enabled ? STAMINA_DISABLE_CMD : STAMINA_ENABLE_CMD);
    LOG(DEBUG) << "setEnabled fail " << file.fail();
    return !file.fail();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor