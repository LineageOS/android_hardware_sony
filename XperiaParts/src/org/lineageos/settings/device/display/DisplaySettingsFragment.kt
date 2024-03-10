/*
 * Copyright (C) 2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device.display

import android.app.AlertDialog
import android.content.Context
import android.os.Bundle
import android.provider.Settings
import android.view.View
import android.widget.CheckBox
import androidx.preference.*

import org.lineageos.settings.device.R

const val CREATOR_MODE_KEY = "switchCreatorMode"

const val CREATOR_MODE_DIALOG_DISABLE_KEY = "creator_mode_dialog_disable"

class DisplaySettingsFragment(private val mContext: Context) : PreferenceFragment(), Preference.OnPreferenceChangeListener {
    private lateinit var creatorModeUtils: CreatorModeUtils

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        addPreferencesFromResource(R.xml.advanced_disp_settings)
        creatorModeUtils = CreatorModeUtils(context)

        val creatorModePreference = findPreference<SwitchPreferenceCompat>(CREATOR_MODE_KEY)!!
        creatorModePreference.isChecked = creatorModeUtils.isEnabled
        creatorModePreference.onPreferenceChangeListener = this
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        val builder: AlertDialog.Builder = AlertDialog.Builder(mContext)
        val checkBoxView = View.inflate(mContext, R.layout.cm_checkbox, null);
        val checkbox = checkBoxView.findViewById<CheckBox>(R.id.cm_checkbox_show_once) as CheckBox
        checkbox.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                Settings.Secure.putInt(context.contentResolver, CREATOR_MODE_DIALOG_DISABLE_KEY, 1)
            } else {
                Settings.Secure.putInt(context.contentResolver, CREATOR_MODE_DIALOG_DISABLE_KEY, 0)
            }
        }

        when (preference.key) {
            CREATOR_MODE_KEY -> {
                if (newValue as Boolean) {
                    if (Settings.Secure.getInt(context.contentResolver,
                                    CREATOR_MODE_DIALOG_DISABLE_KEY, 0) == 0) {
                        builder.setTitle(R.string.cm_dialog_title)
                                .setView(checkBoxView)
                                .setCancelable(false)
                                .setMessage(R.string.cm_dialog_message)
                                .setPositiveButton(android.R.string.ok) { _, _ ->
                                    creatorModeUtils.setMode(true)
                                }
                                .setNegativeButton(android.R.string.cancel) { _, _ ->
                                    preference as SwitchPreferenceCompat
                                    preference.isChecked = !preference.isChecked

                                    Settings.Secure.putInt(context.contentResolver,
                                            CREATOR_MODE_DIALOG_DISABLE_KEY, 0)
                                }
                        builder.show()
                    } else {
                        creatorModeUtils.setMode(true)
                    }
                } else {
                    creatorModeUtils.setMode(false)
                }
            }
        }

        return true
    }
}
