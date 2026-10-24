/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.semc.hardware.aidldisplay;

import vendor.semc.hardware.aidldisplay.PccMatrix;
import vendor.semc.hardware.aidldisplay.PccMatrix3x3;

@VintfStability
interface IDisplayCallback {
    void onWhiteBalanceMatrixChanged(in PccMatrix matrix);
    void onColorMatrixChanged(in PccMatrix3x3 matrix);
}
