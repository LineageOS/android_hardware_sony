/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android-base/file.h>
#include <aidl/vendor/sony/charger/BnCharger.h>

namespace aidl {
namespace vendor {
namespace sony {
namespace charger {

struct Charger : public BnCharger {
    ndk::ScopedAStatus setChargingEnable(bool enabled) override;
    ndk::ScopedAStatus isChargingEnabled(bool *_aidl_return) override;
    ndk::ScopedAStatus setChargingLimit(int32_t limit) override;
};

}   // namespace charger
}   // namespace sony
}   // namespace vendor
}   // namespace aidl
