/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-service-sony"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/ProcessState.h>
#include <livedisplay/sony/SunlightEnhancement.h>
#include <livedisplay/sdm/PictureAdjustment.h>

using ::aidl::vendor::lineage::livedisplay::SunlightEnhancement;
using ::aidl::vendor::lineage::livedisplay::sdm::PictureAdjustment;
using ::aidl::vendor::lineage::livedisplay::sdm::SDMController;

int main() {
    android::ProcessState::self()->setThreadPoolMaxThreadCount(1);
    android::ProcessState::self()->startThreadPool();

    LOG(INFO) << "LiveDisplay HAL service is starting.";

    std::shared_ptr<SDMController> controller = std::make_shared<SDMController>();

    std::shared_ptr<PictureAdjustment> pa = ndk::SharedRefBase::make<PictureAdjustment>(controller);
    std::shared_ptr<SunlightEnhancement> se = ndk::SharedRefBase::make<SunlightEnhancement>();

    std::string inst_pa = std::string() + PictureAdjustment::descriptor + "/default";
    binder_status_t status_pa = AServiceManager_addService(pa->asBinder().get(), inst_pa.c_str());
    CHECK_EQ(status_pa, STATUS_OK);

    std::string inst_se = std::string() + SunlightEnhancement::descriptor + "/default";
    binder_status_t status_se = AServiceManager_addService(se->asBinder().get(), inst_se.c_str());
    CHECK_EQ(status_se, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
