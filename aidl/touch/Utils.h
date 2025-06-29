/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android-base/file.h>

inline auto send_cmd(const char *path, const char* cmd) {
    return android::base::WriteStringToFile(cmd, path);
}

inline auto send_cmd_get_result(const char *wpath, const char *rpath, const char* cmd, std::string& result) {
    return send_cmd(wpath, cmd) && android::base::ReadFileToString(rpath, &result);
}
