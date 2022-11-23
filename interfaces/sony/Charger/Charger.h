/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/sony/charger/BnCharger.h>
#include <android-base/file.h>
#include "android/binder_status.h"

namespace aidl {
namespace vendor {
namespace sony {
namespace charger {

#define CHG_STOP_REASON_FORCE (1 << 0)
#define CHG_STOP_REASON_LIMIT (1 << 1)

struct Charger : public BnCharger {
    Charger();
    ndk::ScopedAStatus setChargingEnable(bool enabled) override;
    ndk::ScopedAStatus isChargingEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setChargingLimit(int32_t limit) override;
    ndk::ScopedAStatus checkBatCapacityAndApplyLimit() override;

  private:
    bool updateChargingStatus();
    bool setChargingEnableInternal(bool enabled);
    int chargerBoard;
    int chgLimit;
    int chgStopReason;
};

}  // namespace charger
}  // namespace sony
}  // namespace vendor
}  // namespace aidl
