/*
 * Copyright (C) 2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device.display

import android.content.Context
import android.hardware.display.ColorDisplayManager
import android.os.ServiceManager
import android.provider.Settings
import android.util.Log
import lineageos.hardware.LiveDisplayManager
import vendor.semc.hardware.aidldisplay.IDisplay as IAidlDisplay
import vendor.semc.hardware.aidldisplay.IDisplayCallback as IAidlDisplayCallback
import vendor.semc.hardware.aidldisplay.PccMatrix as AidlPccMatrix
import vendor.semc.hardware.display.V2_0.IDisplay as IHidlDisplay
import vendor.semc.hardware.display.V2_0.IDisplayCallback as IHidlDisplayCallback
import vendor.semc.hardware.display.V2_0.PccMatrix as HidlPccMatrix

class CreatorModeUtils(private val context: Context) {
    private val colorDisplayManager: ColorDisplayManager =
        context.getSystemService(ColorDisplayManager::class.java)
            ?: throw Exception("Display manager is NULL")
    private val semcDisplay: SemcDisplay by lazy {
        val binder = ServiceManager.waitForDeclaredService("${IAidlDisplay.DESCRIPTOR}/default")

        if (binder != null) {
            AidlSemcDisplay(IAidlDisplay.Stub.asInterface(binder))
        } else {
            HidlSemcDisplay(
                IHidlDisplay.getService() ?: throw Exception("SEMC Display HAL not found")
            )
        }
    }
    private val liveDisplayManager: LiveDisplayManager = LiveDisplayManager.getInstance(context)

    val isEnabled: Boolean
        get() = Settings.Secure.getInt(context.contentResolver, CREATOR_MODE_ENABLE, 0) != 0

    fun setMode(enabled: Boolean) {
        semcDisplay.setSsppColorMode(if (enabled) 0 else 1)
        colorDisplayManager.setColorMode(if (enabled) 0 else 3)
        semcDisplay.setColorMode(if (enabled) 0 else 1)

        Settings.Secure.putInt(context.contentResolver, CREATOR_MODE_ENABLE, if (enabled) 1 else 0)
    }

    fun initialize() {
        Log.e(TAG, "Creator Mode controller setup")

        // Don't apply anything if the setting is disabled
        if (isEnabled) {
            setMode(true)
        }
    }

    private fun onWhiteBalanceMatrixChanged(red: Float, green: Float, blue: Float) {
        Log.i(TAG, "New white balance: $red, $green, $blue")
        liveDisplayManager.setColorAdjustment(floatArrayOf(red, green, blue))
    }

    private interface SemcDisplay {
        fun setColorMode(mode: Int)

        fun setSsppColorMode(mode: Int)
    }

    private inner class AidlSemcDisplay(private val service: IAidlDisplay) :
        IAidlDisplayCallback.Stub(), SemcDisplay {
        init {
            service.registerCallback(this)
            service.setup()
        }

        override fun setColorMode(mode: Int) {
            service.set_color_mode(mode)
        }

        override fun setSsppColorMode(mode: Int) {
            service.set_sspp_color_mode(mode)
        }

        override fun onWhiteBalanceMatrixChanged(matrix: AidlPccMatrix) =
            onWhiteBalanceMatrixChanged(matrix.red, matrix.green, matrix.blue)

        override fun getInterfaceVersion() = IAidlDisplayCallback.VERSION

        override fun getInterfaceHash(): String = IAidlDisplayCallback.HASH
    }

    private inner class HidlSemcDisplay(private val service: IHidlDisplay) :
        IHidlDisplayCallback.Stub(), SemcDisplay {
        init {
            service.registerCallback(this)
            service.setup()
        }

        override fun setColorMode(mode: Int) {
            service.set_color_mode(mode)
        }

        override fun setSsppColorMode(mode: Int) {
            service.set_sspp_color_mode(mode)
        }

        override fun onWhiteBalanceMatrixChanged(matrix: HidlPccMatrix) =
            onWhiteBalanceMatrixChanged(matrix.red, matrix.green, matrix.blue)
    }

    companion object {
        private const val TAG = "CreatorModeUtils"
        private const val CREATOR_MODE_ENABLE = "cm_enable"
    }
}
