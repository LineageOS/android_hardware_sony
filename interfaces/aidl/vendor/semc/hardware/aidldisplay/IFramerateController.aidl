/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.semc.hardware.aidldisplay;

import vendor.semc.hardware.aidldisplay.DispStatus;
import vendor.semc.hardware.aidldisplay.HmdMode;

@VintfStability
interface IFramerateController {
    DispStatus set_displayport_mode(int index);
    DispStatus set_hmd_mode(HmdMode mode);
}
