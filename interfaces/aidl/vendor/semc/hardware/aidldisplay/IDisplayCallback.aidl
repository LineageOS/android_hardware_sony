/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.semc.hardware.aidldisplay;

import vendor.semc.hardware.aidldisplay.PccMatrix;

@VintfStability
interface IDisplayCallback {
    void onWhiteBalanceMatrixChanged(in PccMatrix matrix);
}
