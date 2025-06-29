/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "HighTouchPollingRateService_Sec"

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

constexpr const char* kPanelCmdPath = "/sys/devices/virtual/sec/tsp/cmd";
constexpr const char* kPanelCmdResultPath = "/sys/devices/virtual/sec/tsp/cmd_result";

}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

#define SET_STAMINA_CMD "stamina_enable,"
#define GET_STAMINA_CMD "get_stamina_mode"

#define SET_REPORT_RATE_CMD "doze_mode_change,"
#define GET_REPORT_RATE_CMD "get_doze_mode"

ndk::ScopedAStatus HighTouchPollingRate::getEnabled(bool* _aidl_return) {
    std::string i;

    auto stamina_mode = 0;
    auto ret = send_cmd_get_result(kPanelCmdPath, kPanelCmdResultPath, GET_STAMINA_CMD, i);
    auto result = sscanf(i.c_str(), GET_STAMINA_CMD ":%d", &stamina_mode);
    if (!ret || result != 1) {
        LOG(ERROR) << "Failed to read current HighTouchPollingRate state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    auto doze_mode = 0, rate_mode = 0;
    ret = send_cmd_get_result(kPanelCmdPath, kPanelCmdResultPath, GET_REPORT_RATE_CMD, i);
    result = sscanf(i.c_str(), GET_REPORT_RATE_CMD ":%d,%d", &doze_mode, &rate_mode);

    LOG(INFO) << "Got stamina_mode: " << stamina_mode << " doze_mode: " << doze_mode
              << " rate_mode: " << rate_mode;
    *_aidl_return = ret && result == 2 && stamina_mode == 0 && rate_mode > 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus HighTouchPollingRate::setEnabled(bool enabled) {
    auto is_low_rate_device =
            android::base::GetBoolProperty("ro.vendor.display.low_touch_rate", false);
    LOG(INFO) << "Current device is low_rate: " << is_low_rate_device;

    bool result;
    if (is_low_rate_device) {
        if (enabled)
            result = send_cmd(kPanelCmdPath, SET_STAMINA_CMD "0");
        else
            result = send_cmd(kPanelCmdPath, SET_STAMINA_CMD "1");
    } else {
        result = send_cmd(kPanelCmdPath, SET_STAMINA_CMD "0");
        if (enabled)
            result &= send_cmd(kPanelCmdPath, SET_REPORT_RATE_CMD "2");
        else
            result &= send_cmd(kPanelCmdPath, SET_REPORT_RATE_CMD "1");
    }

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
