/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS IMMUTABLE. DO NOT EDIT IN ANY CASE.                          //
///////////////////////////////////////////////////////////////////////////////

// This file is a snapshot of an AIDL file. Do not edit it manually. There are
// two cases:
// 1). this is a frozen version file - do not edit this in any case.
// 2). this is a 'current' file. If you make a backwards compatible change to
//     the interface (from the latest frozen version), the build system will
//     prompt you to update this file with `m <name>-update-api`.
//
// You must not make a backward incompatible change to any AIDL file built
// with the aidl_interface module type with versions property set. The module
// type is used to build AIDL files in a way that they can be used across
// independently updatable components of the system. If a device is shipped
// with such a backward incompatible change, it has a high risk of breaking
// later when a module using the interface is updated, e.g., Mainline modules.

package vendor.semc.hardware.aidldisplay;
@VintfStability
interface IDisplay {
  vendor.semc.hardware.aidldisplay.PccMatrix get_primary_pcc_matrix();
  vendor.semc.hardware.aidldisplay.PccLimits get_wp_limits_pcc();
  vendor.semc.hardware.aidldisplay.DispStatus notify_ambientlight_value(double value);
  vendor.semc.hardware.aidldisplay.DispStatus notify_hbm_status(int status);
  vendor.semc.hardware.aidldisplay.DispStatus notify_sbm_status(int status);
  void registerCallback(vendor.semc.hardware.aidldisplay.IDisplayCallback callback);
  vendor.semc.hardware.aidldisplay.DispStatus save_primary_pcc_matrix(in vendor.semc.hardware.aidldisplay.PccMatrix cc_matrix);
  vendor.semc.hardware.aidldisplay.DispStatus set_color_mode(vendor.semc.hardware.aidldisplay.ColorMode mode_id);
  vendor.semc.hardware.aidldisplay.DispStatus set_hdr_tm_mode(boolean enable);
  vendor.semc.hardware.aidldisplay.DispStatus set_primary_pcc_matrix(in vendor.semc.hardware.aidldisplay.PccMatrix cc_matrix);
  vendor.semc.hardware.aidldisplay.DispStatus set_sspp_color_mode(vendor.semc.hardware.aidldisplay.SsppColorMode mode_id);
  vendor.semc.hardware.aidldisplay.DispStatus setup();
  vendor.semc.hardware.aidldisplay.DispStatus notify_ambientlight_values(in float[] values);
  int get_interface_version();
  vendor.semc.hardware.aidldisplay.DispStatus notify_adaptive_color_mode_status(boolean enable);
}
