/*
 * Copyright (c) 2025 Alcatraz323 <alcatraz32323@gmail.com>
 * Copyright (C) 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
package org.lineageos.settings.device.mediavib

import android.os.Bundle
import android.widget.CompoundButton
import androidx.preference.ListPreference
import androidx.preference.Preference
import androidx.preference.PreferenceFragment
import com.android.settingslib.widget.MainSwitchPreference
import org.lineageos.settings.device.R
import org.lineageos.settings.device.mediavib.MediaVibration.getMediaVibrationLevel
import org.lineageos.settings.device.mediavib.MediaVibration.getMediaVibrationState
import org.lineageos.settings.device.mediavib.MediaVibration.setMediaVibrationLevel
import org.lineageos.settings.device.mediavib.MediaVibration.switchMediaVibration

class MediaVibrationSettingsFragment : PreferenceFragment(),
    Preference.OnPreferenceChangeListener,
    CompoundButton.OnCheckedChangeListener {
    private lateinit var mSwitchBar: MainSwitchPreference
    private lateinit var mLevelPref: ListPreference

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        addPreferencesFromResource(R.xml.media_vibration_settings)
        val enable = getMediaVibrationState(context)

        mSwitchBar = findPreference<Preference>(PREF_MEDIA_VIB_ENABLE) as MainSwitchPreference
        mSwitchBar.addOnSwitchChangeListener(this)
        mSwitchBar.isChecked = enable

        mLevelPref = findPreference<Preference>(PREF_MEDIA_VIB_LEVEL) as ListPreference
        mLevelPref.onPreferenceChangeListener = this
        mLevelPref.isEnabled = enable
        mLevelPref.setValueIndex(getMediaVibrationLevel(context))
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any): Boolean {
        when (preference.key) {
            PREF_MEDIA_VIB_LEVEL -> {
                setMediaVibrationLevel(context, newValue.toString().toInt())
                return true
            }

            else -> return false
        }
    }

    override fun onCheckedChanged(buttonView: CompoundButton, isChecked: Boolean) {
        mSwitchBar.isChecked = isChecked
        mLevelPref.isEnabled = isChecked
        switchMediaVibration(context, isChecked)
    }

    companion object {
        const val PREF_MEDIA_VIB_ENABLE: String = "mediavib_enable"
        const val PREF_MEDIA_VIB_LEVEL: String = "mediavib_level"
    }
}