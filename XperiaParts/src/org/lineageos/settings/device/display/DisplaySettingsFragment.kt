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

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        addPreferencesFromResource(R.xml.advanced_disp_settings)
        creatorModeUtils = CreatorModeUtils(context)

        val creatorModePreference = findPreference<SwitchPreference>(CREATOR_MODE_KEY)!!
        creatorModePreference.isChecked = creatorModeUtils.isEnabled
        creatorModePreference.onPreferenceChangeListener = this
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        when(preference.key) {
            CREATOR_MODE_KEY -> creatorModeUtils.setMode(newValue as Boolean)
        }

        return true
    }
}
