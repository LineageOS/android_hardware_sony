/*
 * Copyright (C) 2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.util.Log

import org.lineageos.settings.device.display.CreatorModeUtils
import org.lineageos.settings.device.mediavib.MediaVibration
import org.lineageos.settings.device.volsync.VolumeListenerService

class BootCompletedReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        Log.d(TAG, "Starting")
        CreatorModeUtils(context).initialize()
        if (MediaVibration.isMediaVibAvailable(context)) {
            MediaVibration.enableActivity(context)
            MediaVibration.onBoot(context)
        } else {
            // Remove this activity if the feature is not available
            MediaVibration.disableActivity(context)
            Log.i(TAG, "Media vibration is not available, removing the activity")
        }
        VolumeListenerService.onBoot(context)
    }

    companion object {
        private const val TAG = "XperiaParts"
    }
}
