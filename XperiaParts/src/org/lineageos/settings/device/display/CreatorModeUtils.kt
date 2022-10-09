/*
 * Copyright (C) 2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device.display

import android.content.Context
import android.hardware.display.ColorDisplayManager
import android.provider.Settings
import android.util.Log

import lineageos.hardware.LiveDisplayManager

import vendor.semc.hardware.display.V2_0.IDisplay
import vendor.semc.hardware.display.V2_0.IDisplayCallback
import vendor.semc.hardware.display.V2_0.PccMatrix

class CreatorModeUtils(private val context: Context) : IDisplayCallback.Stub() {
    private val colorDisplayManager: ColorDisplayManager =
            context.getSystemService(ColorDisplayManager::class.java)
                    ?: throw Exception("Display manager NULL")
    private val semcDisplayService: IDisplay =
            IDisplay.getService() ?: throw Exception("SEMC Display HIDL Not found")
    private val liveDisplayManager: LiveDisplayManager = LiveDisplayManager.getInstance(context)

    fun enableCM(context: Context) {
        semcDisplayService.set_sspp_color_mode(0)
        colorDisplayManager.setColorMode(0)
        semcDisplayService.set_color_mode(0)

        Settings.Secure.putInt(context.contentResolver, CREATOR_MODE_ENABLE, 1)
    }

    fun disableCM(context: Context) {
        semcDisplayService.set_sspp_color_mode(1)
        colorDisplayManager.setColorMode(3)
        semcDisplayService.set_color_mode(1)

        Settings.Secure.putInt(context.contentResolver, CREATOR_MODE_ENABLE, 0)
    }

    fun initialize(): Boolean {
        Log.e(TAG, "Creator Mode controller setup")

        // Register itself as callback for HIDL
        semcDisplayService.registerCallback(this)
        semcDisplayService.setup()

        return true
    }

    override fun onWhiteBalanceMatrixChanged(matrix: PccMatrix) {
        val colorMatrix: FloatArray = floatArrayOf(matrix.red, matrix.green, matrix.blue)
        Log.i(TAG, "New White Balance: " + matrix.red + ", " + matrix.green + ", " + matrix.blue)
        liveDisplayManager.setColorAdjustment(colorMatrix)
    }

    companion object {
        private const val TAG = "CreatorModeUtils"
        private const val CREATOR_MODE_ENABLE = "cm_enable"

        fun isCMEnabled(context: Context): Boolean {
            return Settings.Secure.getInt(context.contentResolver, CREATOR_MODE_ENABLE, 0) != 0
        }
    }
}
