/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Charger.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using ::aidl::vendor::sony::charger::Charger;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<Charger> chg = ndk::SharedRefBase::make<Charger>();

    const std::string instance  = std::string() + Charger::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(chg->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;    // should not reach
}
