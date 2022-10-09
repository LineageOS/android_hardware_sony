/*
 * Copyright (C) 2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device.display;

import android.os.Bundle
import androidx.preference.*

import org.lineageos.settings.device.R

const val CREATOR_MODE_KEY = "switchCreatorMode"

class DisplaySettingsFragment : PreferenceFragment(), Preference.OnPreferenceChangeListener {
    private var mCMCtrl: CMUtils? = null
    private var creatorModePreference: SwitchPreference? = null

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        addPreferencesFromResource(R.xml.advanced_disp_settings)
        this.mCMCtrl = CMUtils(context)

        creatorModePreference = findPreference(CREATOR_MODE_KEY)
        creatorModePreference?.isChecked = CMUtils.isCMEnabled(context)
        creatorModePreference?.onPreferenceChangeListener = this
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        if (preference.key == CREATOR_MODE_KEY) {
            if (newValue as Boolean) {
                mCMCtrl?.enableCM(context)
            } else {
                mCMCtrl?.disableCM(context)
            }
        }

        return true
    }
}
