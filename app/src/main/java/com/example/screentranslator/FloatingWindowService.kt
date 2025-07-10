package com.example.screentranslator

import android.app.Service
import android.content.Context
import android.content.Intent
import android.graphics.PixelFormat
import android.os.Build
import android.os.IBinder
import android.view.*
import android.widget.ImageView
import android.widget.Toast

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

        // Floating Button (Overlay icon)
        floatingButton = ImageView(this).apply {
            setImageResource(android.R.drawable.ic_menu_search)
            setOnClickListener { performOCRAndTranslate() }
            setOnLongClickListener {
                Toast.makeText(this@FloatingWindowService, "Sentuhan lama terdeteksi!", Toast.LENGTH_SHORT).show()
                true
            }
            setOnTouchListener(DragTouchListener())
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

    // Listener agar floating button bisa di-drag
    inner class DragTouchListener : View.OnTouchListener {
        private var initialX = 0
        private var initialY = 0
        private var initialTouchX = 0f
        private var initialTouchY = 0f
        override fun onTouch(v: View?, event: MotionEvent): Boolean {
            val params = floatingButton.layoutParams as WindowManager.LayoutParams
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    initialX = params.x
                    initialY = params.y
                    initialTouchX = event.rawX
                    initialTouchY = event.rawY
                    return false // allow click and long press to work
                }
                MotionEvent.ACTION_MOVE -> {
                    params.x = initialX + (event.rawX - initialTouchX).toInt()
                    params.y = initialY + (event.rawY - initialTouchY).toInt()
                    windowManager.updateViewLayout(floatingButton, params)
                    return true
                }
            }
            return false
        }
    }
}
