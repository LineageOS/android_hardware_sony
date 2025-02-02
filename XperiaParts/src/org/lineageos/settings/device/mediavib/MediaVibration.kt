/*
 * Copyright (c) 2025 Alcatraz323 <alcatraz32323@gmail.com>
 * Copyright (C) 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
package org.lineageos.settings.device.mediavib

import android.content.ComponentName
import android.content.Context
import android.content.pm.PackageManager
import android.media.AudioManager
import android.provider.Settings

object MediaVibration {
    fun switchMediaVibration(context: Context, enable: Boolean) {
        val am = context.getSystemService(
            AudioManager::class.java
        )
        val paramStr = if (enable) "1" else "0"
        am!!.setParameters("somc.media_vibration=$paramStr")
        Settings.Secure.putInt(
            context.contentResolver,
            MediaVibrationSettingsFragment.PREF_MEDIA_VIB_ENABLE,
            if (enable) 1 else 0
        )
    }

    fun isMediaVibAvailable(context: Context): Boolean {
        val am = context.getSystemService(
            AudioManager::class.java
        )
        val ret = am!!.getParameters("somc.media_vibration") != ""
        return ret
    }

    fun disableActivity(context: Context) {
        val pm = context.packageManager
        pm.setComponentEnabledSetting(
            ComponentName(context, MediaVibrationActivity::class.java),
            PackageManager.COMPONENT_ENABLED_STATE_DISABLED,
            PackageManager.DONT_KILL_APP
        )
    }

    fun enableActivity(context: Context) {
        val pm = context.packageManager
        pm.setComponentEnabledSetting(
            ComponentName(context, MediaVibrationActivity::class.java),
            PackageManager.COMPONENT_ENABLED_STATE_ENABLED,
            PackageManager.DONT_KILL_APP
        )
    }

    fun setMediaVibrationLevel(context: Context, level: Int) {
        val am = context.getSystemService(
            AudioManager::class.java
        )
        am!!.setParameters("somc.media_vibration_vol_idx=$level")
        Settings.Secure.putInt(
            context.contentResolver,
            MediaVibrationSettingsFragment.PREF_MEDIA_VIB_LEVEL,
            level
        )
    }

    fun setMediaVibrationLatency(context: Context, latency: Int) {
        val am = context.getSystemService(
            AudioManager::class.java
        )
        am!!.setParameters("somc.media_vibration_bt_delay=${500 - latency}")
        Settings.Secure.putInt(
            context.contentResolver,
            MediaVibrationLatencyPreference.PREF_MEDIA_VIB_LATENCY,
            latency
        )
    }

    fun getMediaVibrationState(context: Context): Boolean {
        val state =
            Settings.Secure.getInt(
                context.contentResolver,
                MediaVibrationSettingsFragment.PREF_MEDIA_VIB_ENABLE,
                0
            ) == 1
        return state
    }

    fun getMediaVibrationLevel(context: Context): Int {
        val level =
            Settings.Secure.getInt(
                context.contentResolver,
                MediaVibrationSettingsFragment.PREF_MEDIA_VIB_LEVEL,
                0
            )
        return level
    }

    fun getMediaVibrationLatency(context: Context): Int {
        val latency =
            Settings.Secure.getInt(
                context.contentResolver,
                MediaVibrationLatencyPreference.PREF_MEDIA_VIB_LATENCY,
                0
            )
        return latency
    }

    fun onBoot(context: Context) {
        switchMediaVibration(context, getMediaVibrationState(context))
        setMediaVibrationLevel(context, getMediaVibrationLevel(context))
    }
}