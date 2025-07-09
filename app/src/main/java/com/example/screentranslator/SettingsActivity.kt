package com.example.screentranslator

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.text.InputType
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.SwitchCompat

class SettingsActivity : AppCompatActivity() {

    private lateinit var intervalInput: EditText
    private lateinit var autoSwitch: SwitchCompat
    private lateinit var sourceLangSpinner: Spinner
    private lateinit var targetLangSpinner: Spinner

    private val languages = listOf(
        "Auto" to "auto",
        "English" to "en",
        "Indonesian" to "id",
        "Japanese" to "ja",
        "Korean" to "ko",
        "Chinese" to "zh",
        "Spanish" to "es"
    )

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val prefs = getSharedPreferences("translator_prefs", Context.MODE_PRIVATE)

        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(40, 40, 40, 40)

            addView(TextView(context).apply {
                text = "Interval (detik):"
            })

            intervalInput = EditText(context).apply {
                inputType = InputType.TYPE_CLASS_NUMBER
            }
            addView(intervalInput)

            autoSwitch = SwitchCompat(context).apply {
                text = "Auto Translate"
            }
            addView(autoSwitch)

            sourceLangSpinner = Spinner(context)
            targetLangSpinner = Spinner(context)

            val langNames = languages.map { it.first }
            sourceLangSpinner.adapter = ArrayAdapter(context, android.R.layout.simple_spinner_dropdown_item, langNames)
            targetLangSpinner.adapter = ArrayAdapter(context, android.R.layout.simple_spinner_dropdown_item, langNames)

            addView(TextView(context).apply { text = "Source Language:" })
            addView(sourceLangSpinner)

            addView(TextView(context).apply { text = "Target Language:" })
            addView(targetLangSpinner)

            addView(Button(context).apply {
                text = "Simpan"
                setOnClickListener {
                    val interval = intervalInput.text.toString().toLongOrNull() ?: 10
                    val sourceLang = languages[sourceLangSpinner.selectedItemPosition].second
                    val targetLang = languages[targetLangSpinner.selectedItemPosition].second

                    prefs.edit().apply {
                        putLong("interval", interval * 1000)
                        putBoolean("auto_mode", autoSwitch.isChecked)
                        putString("source_lang", sourceLang)
                        putString("target_lang", targetLang)
                        apply()
                    }
                    Toast.makeText(context, "Tersimpan", Toast.LENGTH_SHORT).show()
                }
            })

            addView(Button(context).apply {
                text = "Mulai Overlay"
                setOnClickListener {
                    startService(Intent(context, FloatingWindowService::class.java))
                }
            })

            addView(Button(context).apply {
                text = "Stop Overlay"
                setOnClickListener {
                    stopService(Intent(context, FloatingWindowService::class.java))
                }
            })
        }

        setContentView(layout)

        // Load saved settings
        val interval = prefs.getLong("interval", 10000L)
        intervalInput.setText((interval / 1000).toString())
        autoSwitch.isChecked = prefs.getBoolean("auto_mode", true)
        sourceLangSpinner.setSelection(languages.indexOfFirst { it.second == prefs.getString("source_lang", "auto") })
        targetLangSpinner.setSelection(languages.indexOfFirst { it.second == prefs.getString("target_lang", "id") })
    }
}