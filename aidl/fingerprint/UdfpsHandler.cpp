/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "UdfpsHandler.h"
#include <dlfcn.h>

UdfpsHandlerFactory* getUdfpsHandlerFactory() {
    // Unsupported usecase on sony
    return nullptr;
}
