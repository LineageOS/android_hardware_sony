/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.semc.hardware.aidldisplay;

import vendor.semc.hardware.aidldisplay.ColorMode;
import vendor.semc.hardware.aidldisplay.DispStatus;
import vendor.semc.hardware.aidldisplay.IDisplayCallback;
import vendor.semc.hardware.aidldisplay.PccLimits;
import vendor.semc.hardware.aidldisplay.PccMatrix;
import vendor.semc.hardware.aidldisplay.SsppColorMode;

@VintfStability
interface IDisplay {
    PccMatrix get_primary_pcc_matrix();
    PccLimits get_wp_limits_pcc();
    DispStatus notify_ambientlight_value(double value);
    DispStatus notify_hbm_status(int status);
    DispStatus notify_sbm_status(int status);
    void registerCallback(IDisplayCallback callback);
    DispStatus save_primary_pcc_matrix(in PccMatrix cc_matrix);
    DispStatus set_color_mode(ColorMode mode_id);
    DispStatus set_hdr_tm_mode(boolean enable);
    DispStatus set_primary_pcc_matrix(in PccMatrix cc_matrix);
    DispStatus set_sspp_color_mode(SsppColorMode mode_id);
    DispStatus setup();
    DispStatus notify_ambientlight_values(in float[] values);
    int get_interface_version();
    DispStatus notify_adaptive_color_mode_status(boolean enable);
}
