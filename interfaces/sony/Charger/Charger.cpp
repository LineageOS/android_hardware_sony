/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Charger.h"

#include <android-base/logging.h>
#include <android/binder_status.h>

namespace aidl {
namespace vendor {
namespace sony {
namespace charger {

ndk::ScopedAStatus Charger::isChargingEnabled(bool *_aidl_return) {
  LOG(VERBOSE) << "Charging enabled status: ";
  *_aidl_return = true;

  return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Charger::setChargingEnable(bool enabled) {
  LOG(VERBOSE) << (enabled ? "Enable" : "Disable") << " charging";

  return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Charger::setChargingLimit(int32_t limit) {
  LOG(VERBOSE) << "Setting charging limit to " << limit;
  if (limit > 100 || limit <= 50) {
    LOG(ERROR) << "The charging limit " << limit << " is not supported!";
    LOG(ERROR) << "  Please select between 51 and 100";
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
  }

  if (limit == 100) {
    return setChargingEnable(true);
  }

  return ndk::ScopedAStatus::ok();
}
} // namespace charger
} // namespace sony
} // namespace vendor
} // namespace aidl
