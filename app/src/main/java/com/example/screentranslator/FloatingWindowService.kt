package com.example.screentranslator

import android.app.Service
import android.content.Intent
import android.os.IBinder
import android.view.*
import android.graphics.PixelFormat
import android.os.Build
import android.widget.ImageView

class FloatingWindowService : Service() {

    private lateinit var windowManager: WindowManager
    private lateinit var selectorView: OverlayView
    private lateinit var resultView: ResultOverlayView
    private lateinit var floatingButton: ImageView
    private lateinit var timerManager: TimerManager

    private lateinit var resultParams: WindowManager.LayoutParams
    private lateinit var prefs: android.content.SharedPreferences

    override fun onCreate() {
        super.onCreate()
        windowManager = getSystemService(WINDOW_SERVICE) as WindowManager
        prefs = getSharedPreferences("translator_prefs", Context.MODE_PRIVATE)

        val intervalMs = prefs.getLong("interval", 10000L)
        val autoTranslate = prefs.getBoolean("auto_mode", true)

        // Selector View
        selectorView = OverlayView(this)
        val selectorParams = WindowManager.LayoutParams(
            WindowManager.LayoutParams.MATCH_PARENT,
            WindowManager.LayoutParams.MATCH_PARENT,
            overlayType(),
            WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL or WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT
        )
        windowManager.addView(selectorView, selectorParams)

        // Result View
        resultView = ResultOverlayView(this)
        resultParams = WindowManager.LayoutParams(
            600, 300,
            overlayType(),
            WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL or WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT
        )
        resultParams.gravity = Gravity.TOP or Gravity.END
        resultParams.x = 50
        resultParams.y = 300
        resultView.attachToWindow(resultParams)

        // Floating Button
        floatingButton = ImageView(this).apply {
            setImageResource(android.R.drawable.ic_menu_search)
            setOnClickListener { performOCRAndTranslate() }
        }

        val buttonParams = WindowManager.LayoutParams(
            150, 150,
            overlayType(),
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT
        )
        buttonParams.gravity = Gravity.TOP or Gravity.START
        buttonParams.x = 200
        buttonParams.y = 300
        windowManager.addView(floatingButton, buttonParams)

        timerManager = TimerManager(intervalMs) {
            performOCRAndTranslate()
        }
        if (autoTranslate) timerManager.start()
    }

    private fun overlayType(): Int {
        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY
        else WindowManager.LayoutParams.TYPE_PHONE
    }

    private fun performOCRAndTranslate() {
        ScreenshotUtil.captureScreen(this) { bitmap ->
            val rect = selectorView.getSelectedRect()
            val cropped = android.graphics.Bitmap.createBitmap(
                bitmap,
                rect.left,
                rect.top,
                rect.width(),
                rect.height()
            )
            val sourceLang = prefs.getString("source_lang", "auto") ?: "auto"
            val targetLang = prefs.getString("target_lang", "id") ?: "id"

            OCRProcessor.recognizeText(cropped) { text ->
                Translator.translate(text, sourceLang, targetLang) { translated ->
                    resultView.updateText(translated)
                }
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        timerManager.stop()
        windowManager.removeView(selectorView)
        windowManager.removeView(floatingButton)
        resultView.remove()
    }

    override fun onBind(intent: Intent?): IBinder? = null
}
