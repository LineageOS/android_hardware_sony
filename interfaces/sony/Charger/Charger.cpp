/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Charger.h"

#include <android-base/logging.h>
#include <android/binder_status.h>
#include <cutils/properties.h>
#include <cutils/uevent.h>
#include <string.h>
#include <fstream>

namespace aidl {
namespace vendor {
namespace sony {
namespace charger {

#define LOG_TAG "vendor.sony.charger"

enum SUPPORTED_BOARD {
    kona = 0,
    lahaina = 1,
};

static constexpr const char* kChgActivationPath[] = {
        "/sys/class/power_supply/battery_ext/smart_charging_activation",
        "/sys/class/battchg_ext/smart_charging_activation",
};

static constexpr const char* kChgInterruptionPath[] = {
        "/sys/class/power_supply/battery_ext/smart_charging_interruption",
        "/sys/class/battchg_ext/smart_charging_interruption",
};

static constexpr const char* kBatCapacityPath[] = {
        "/sys/class/power_supply/battery/capacity",
        "/sys/class/power_supply/battery/capacity",
};

Charger::Charger() {
    LOG(INFO) << "Charger HAL Starting...";
    char board_name[PROPERTY_VALUE_MAX];

    property_get("ro.product.board", board_name, "null");

    if (strncmp(board_name, "kona", strlen("kona")) == 0) {
        chargerBoard = kona;
    } else if (strncmp(board_name, "lahaina", strlen("lahaina")) == 0) {
        chargerBoard = lahaina;
    } else {
        LOG(ERROR) << "Failed to get board number, default to kona";
        chargerBoard = kona;
    }
    chgLimit = 100;
    chgStopReason = 0;
}

bool Charger::updateChargingStatus() {
    // If chgStopReason is not 0, we need to stop charging
    bool enable = chgStopReason == 0;

    return setChargingEnableInternal(enable);
}

bool Charger::setChargingEnableInternal(bool enabled) {
    LOG(INFO) << (enabled ? "Enable" : "Disable") << " charging";
    std::fstream activationFile(kChgActivationPath[chargerBoard]);
    std::fstream interruptionFile(kChgInterruptionPath[chargerBoard]);
    int activationResult = -1;
    int interruptionResult = -1;

    if (!activationFile.is_open() || !interruptionFile.is_open()) goto error;

    activationFile >> activationResult;
    interruptionFile >> interruptionResult;

    if (activationFile.fail() || interruptionFile.fail()) goto error;

    if (activationResult == 0) {
        activationFile << "1";
        if (activationFile.fail()) goto error;
    }

    if (interruptionResult == enabled) {
        interruptionFile << (enabled ? "0" : "1");
        if (interruptionFile.fail()) goto error;
    }

    interruptionFile.close();
    activationFile.close();

    return true;

error:
    LOG(ERROR) << "Failed to read or write charging status: " << strerror(errno);
    return false;
}

ndk::ScopedAStatus Charger::isChargingEnabled(bool* _aidl_return) {
    *_aidl_return = chgStopReason & CHG_STOP_REASON_FORCE;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Charger::setChargingEnable(bool enabled) {
    if (enabled)
        chgStopReason &= ~CHG_STOP_REASON_FORCE;  // clear bit, enable charging
    else
        chgStopReason |= CHG_STOP_REASON_FORCE;  // Set bit, disable charging

    if (updateChargingStatus())
        return ndk::ScopedAStatus::ok();
    else
        return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
}

ndk::ScopedAStatus Charger::setChargingLimit(int32_t limit) {
    LOG(INFO) << "Setting charging limit to " << limit;
    if (limit > 100 || limit < 50) {
        LOG(ERROR) << "The charging limit " << limit << " is not supported!";
        LOG(ERROR) << "  Please select between 50 and 100";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    chgLimit = limit;

    return checkBatCapacityAndApplyLimit();
}

ndk::ScopedAStatus Charger::checkBatCapacityAndApplyLimit() {
    std::ifstream batCapFile(kBatCapacityPath[chargerBoard]);
    int batCapNow = -1;

    batCapFile >> batCapNow;
    LOG(INFO) << "Capacity: " << batCapNow << ". Limit: " << chgLimit;

    if (chgLimit == 100) {
        // We don't limit battery if the limit is 100
        chgStopReason &= ~CHG_STOP_REASON_LIMIT;
    } else {
        if (!batCapFile.is_open()) goto error;

        if (batCapNow >= chgLimit) {
            // Set limit
            chgStopReason |= CHG_STOP_REASON_LIMIT;
        } else {
            chgStopReason &= ~CHG_STOP_REASON_LIMIT;
        }
    }

    if (updateChargingStatus())
        return ndk::ScopedAStatus::ok();
    else
        return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);

error:
    LOG(ERROR) << "Failed to read charging status: " << strerror(errno);
    return ndk::ScopedAStatus::fromExceptionCode(EX_SERVICE_SPECIFIC);
}

}  // namespace charger
}  // namespace sony
}  // namespace vendor
}  // namespace aidl
