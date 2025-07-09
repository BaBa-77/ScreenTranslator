package com.example.screentranslator

import android.os.Handler
import android.os.Looper

class TimerManager(
    private val intervalMillis: Long,
    private val onTrigger: () -> Unit
) {
    private val handler = Handler(Looper.getMainLooper())
    private val runnable = object : Runnable {
        override fun run() {
            onTrigger()
            handler.postDelayed(this, intervalMillis)
        }
    }

    fun start() {
        handler.post(runnable)
    }

    fun stop() {
        handler.removeCallbacks(runnable)
    }
}