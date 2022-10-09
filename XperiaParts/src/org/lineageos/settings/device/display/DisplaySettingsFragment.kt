/*
 * Copyright (C) 2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device.display

import android.os.Bundle
import androidx.preference.*

import org.lineageos.settings.device.R

const val CREATOR_MODE_KEY = "switchCreatorMode"

class DisplaySettingsFragment : PreferenceFragment(), Preference.OnPreferenceChangeListener {
    private lateinit var creatorModeUtils: CreatorModeUtils
    private lateinit var creatorModePreference: SwitchPreference

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        addPreferencesFromResource(R.xml.advanced_disp_settings)
        this.creatorModeUtils = CreatorModeUtils(context)

        creatorModePreference = findPreference(CREATOR_MODE_KEY)!!
        creatorModePreference.isChecked = CreatorModeUtils.isCMEnabled(context)
        creatorModePreference.onPreferenceChangeListener = this
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        if (preference.key == CREATOR_MODE_KEY) {
            if (newValue as Boolean) {
                creatorModeUtils.enableCM(context)
            } else {
                creatorModeUtils.disableCM(context)
            }
        }

        return true
    }
}
