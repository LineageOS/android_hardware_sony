/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "HardwareBase.h"

#include <cutils/properties.h>
#include <dlfcn.h>
#include <log/log.h>

#include <fstream>
#include <sstream>

#include "utils.h"

namespace {

// TA functions
static void* ta_handle = nullptr;
static int (*miscta_get_unit_size)(uint32_t unit, uint32_t* size) = nullptr;
static int (*miscta_read_unit)(uint32_t id, void* buf, uint32_t* size) = nullptr;

template <typename T>
static bool ta_read_unit(T& out, uint32_t unit) {
    uint32_t size = 0;

    int ret = miscta_get_unit_size(unit, &size);
    if (ret) {
        ALOGE("%s: Cannot retrieve TA unit %d size error %d", __func__, unit, ret);
        return false;
    }

    if (size != sizeof(T)) {
        ALOGE("%s: Unexpected TA unit size %d != %lu", __func__, size, sizeof(T));
        return false;
    }

    ret = miscta_read_unit(unit, &out, &size);
    if (ret) {
        ALOGE("%s: Cannot read TA unit %d of size %u: error %d", __func__, unit, size, ret);
        return false;
    }

    return true;
}

};  // namespace

namespace aidl {
namespace android {
namespace hardware {
namespace vibrator {

HwApiBase::HwApiBase() {
    mPathPrefix = std::getenv("HWAPI_PATH_PREFIX") ?: "";
    if (mPathPrefix.empty()) {
        ALOGE("Failed get HWAPI path prefix!");
    }
}

void HwApiBase::saveName(const std::string& name, const std::ios* stream) {
    mNames[stream] = name;
}

bool HwApiBase::has(const std::ios& stream) {
    return !!stream;
}

void HwApiBase::debug(int fd) {
    dprintf(fd, "Kernel:\n");

    for (auto& entry : utils::pathsFromEnv("HWAPI_DEBUG_PATHS", mPathPrefix)) {
        auto& path = entry.first;
        auto& stream = entry.second;
        std::string line;

        dprintf(fd, "  %s:\n", path.c_str());
        while (std::getline(stream, line)) {
            dprintf(fd, "    %s\n", line.c_str());
        }
    }

    mRecordsMutex.lock();
    dprintf(fd, "  Records:\n");
    for (auto& r : mRecords) {
        if (r == nullptr) {
            continue;
        }
        dprintf(fd, "    %s\n", r->toString(mNames).c_str());
    }
    mRecordsMutex.unlock();
}

HwCalBase::HwCalBase() {
    std::ifstream calfile;
    auto propertyPrefix = std::getenv("PROPERTY_PREFIX");

    if (propertyPrefix != NULL) {
        mPropertyPrefix = std::string(propertyPrefix);
    } else {
        ALOGE("Failed get property prefix!");
    }

    ta_handle = dlopen("libmiscta.so", RTLD_NOW);
    if (ta_handle) {
        // Load related symbols
        miscta_get_unit_size = reinterpret_cast<typeof(miscta_get_unit_size)>(
                dlsym(ta_handle, "miscta_get_unit_size"));
        if (!miscta_get_unit_size) {
            ALOGE("%s: Cannot find symbol: miscta_get_unit_size", __func__);
            return;
        }

        miscta_read_unit =
                reinterpret_cast<typeof(miscta_read_unit)>(dlsym(ta_handle, "miscta_read_unit"));
        if (!miscta_read_unit) {
            ALOGE("%s: Cannot find symbol: miscta_read_unit", __func__);
            return;
        }

        int redc_measured = 0;
        int f0_measured = 0;

        if (ta_read_unit(redc_measured, 4732) && ta_read_unit(f0_measured, 4733)) {
            mCalData["f0_measured"] = std::to_string(f0_measured);
            mCalData["redc_measured"] = std::to_string(redc_measured);
        }
    }
}

void HwCalBase::debug(int fd) {
    std::ifstream stream;
    std::string path;
    std::string line;
    struct context {
        HwCalBase* obj;
        int fd;
    } context{this, fd};

    dprintf(fd, "Properties:\n");

    property_list(
            [](const char* key, const char* value, void* cookie) {
                struct context* context = static_cast<struct context*>(cookie);
                HwCalBase* obj = context->obj;
                int fd = context->fd;
                const std::string expect{obj->mPropertyPrefix};
                const std::string actual{key, std::min(strlen(key), expect.size())};
                if (actual == expect) {
                    dprintf(fd, "  %s:\n", key);
                    dprintf(fd, "    %s\n", value);
                }
            },
            &context);

    dprintf(fd, "\n");

    dprintf(fd, "Persist:\n");

    utils::fileFromEnv("CALIBRATION_FILEPATH", &stream, &path);

    dprintf(fd, "  %s:\n", path.c_str());
    while (std::getline(stream, line)) {
        dprintf(fd, "    %s\n", line.c_str());
    }
}

}  // namespace vibrator
}  // namespace hardware
}  // namespace android
}  // namespace aidl
