package com.example.screentranslator

import android.content.Context
import android.graphics.*
import android.view.MotionEvent
import android.view.View

class OverlayView(context: Context) : View(context) {

    private val paint = Paint().apply {
        color = Color.RED
        style = Paint.Style.STROKE
        strokeWidth = 4f
    }

    private var rect = Rect(200, 200, 600, 600)
    private var dragging = false
    private var lastX = 0f
    private var lastY = 0f

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        canvas.drawRect(rect, paint)
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                if (rect.contains(event.x.toInt(), event.y.toInt())) {
                    dragging = true
                    lastX = event.x
                    lastY = event.y
                    return true
                }
            }

            MotionEvent.ACTION_MOVE -> {
                if (dragging) {
                    val dx = (event.x - lastX).toInt()
                    val dy = (event.y - lastY).toInt()
                    rect.offset(dx, dy)
                    lastX = event.x
                    lastY = event.y
                    invalidate()
                    return true
                }
            }

            MotionEvent.ACTION_UP -> dragging = false
        }
        return false
    }

    fun getSelectedRect(): Rect = rect
}