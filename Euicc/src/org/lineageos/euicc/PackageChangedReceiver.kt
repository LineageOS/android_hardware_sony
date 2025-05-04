/*
 * Copyright (C) 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.euicc

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.util.Log

class PackageChangedReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        Log.d(TAG, "Received package changed intent")
        EuiccDisabler.enableOrDisableEuicc(context)
    }

    companion object {
        private const val TAG = "SonyEuiccPackageReceiver"
    }
}
