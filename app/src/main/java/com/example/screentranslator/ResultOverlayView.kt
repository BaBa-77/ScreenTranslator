package com.example.screentranslator

import android.content.Context
import android.graphics.Color
import android.graphics.Typeface
import android.os.Handler
import android.os.Looper
import android.view.*
import android.widget.LinearLayout
import android.widget.TextView

class ResultOverlayView(context: Context) : LinearLayout(context) {

    private val resultTextView: TextView
    private val windowManager = context.getSystemService(Context.WINDOW_SERVICE) as WindowManager
    private lateinit var layoutParams: WindowManager.LayoutParams

    private var lastTouchX = 0f
    private var lastTouchY = 0f

    init {
        orientation = VERTICAL
        setBackgroundColor(Color.parseColor("#AA000000"))
        setPadding(20, 20, 20, 20)

        resultTextView = TextView(context).apply {
            setTextColor(Color.WHITE)
            textSize = 16f
            typeface = Typeface.MONOSPACE
        }
        addView(resultTextView)

        setOnTouchListener { _, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    lastTouchX = event.rawX
                    lastTouchY = event.rawY
                    true
                }
                MotionEvent.ACTION_MOVE -> {
                    val dx = event.rawX - lastTouchX
                    val dy = event.rawY - lastTouchY
                    layoutParams.x += dx.toInt()
                    layoutParams.y += dy.toInt()
                    windowManager.updateViewLayout(this, layoutParams)
                    lastTouchX = event.rawX
                    lastTouchY = event.rawY
                    true
                }
                else -> false
            }
        }
    }

    fun attachToWindow(params: WindowManager.LayoutParams) {
        layoutParams = params
        windowManager.addView(this, layoutParams)
    }

    fun updateText(text: String) {
        resultTextView.text = text
        visibility = View.VISIBLE

        Handler(Looper.getMainLooper()).postDelayed({
            visibility = View.GONE
        }, 5000)
    }

    fun remove() {
        windowManager.removeView(this)
    }
}