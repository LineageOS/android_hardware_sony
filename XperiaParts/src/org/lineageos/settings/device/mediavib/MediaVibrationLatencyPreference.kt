package org.lineageos.settings.device.mediavib

import android.content.Context
import android.util.AttributeSet
import android.widget.SeekBar
import androidx.preference.Preference
import android.widget.TextView
import androidx.preference.PreferenceViewHolder

import org.lineageos.settings.device.R

class MediaVibrationLatencyPreference(context: Context, attrs: AttributeSet) : Preference(
    context,
    attrs
), SeekBar.OnSeekBarChangeListener {
    private lateinit var mLatencyValue: TextView
    private lateinit var mLatencyBar: SeekBar

    init {
        layoutResource = R.layout.preference_media_vibration_latency
    }

    override fun onBindViewHolder(holder: PreferenceViewHolder) {
        super.onBindViewHolder(holder)

        mLatencyValue = holder.findViewById(R.id.latency_value)!! as TextView
        mLatencyBar = holder.findViewById(R.id.latency_seekbar)!! as SeekBar
        mLatencyBar.setOnSeekBarChangeListener(this)

        val latency = MediaVibration.getMediaVibrationLatency(context)
        mLatencyBar.progress = latency
    }

    override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
        mLatencyValue.text = String.format("%d", progress)
    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {
    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {
        MediaVibration.setMediaVibrationLatency(context, seekBar!!.progress)
    }

    companion object {
        const val PREF_MEDIA_VIB_LATENCY: String = "mediavib_latency"
    }

}