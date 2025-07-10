package com.example.screentranslator

import android.app.Activity
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.provider.Settings
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.FrameLayout
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    private val projectionRequestCode = 1234

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val button = Button(this).apply {
        text = "Mulai Layanan"
        // ...
        }

        button.setOnClickListener {
            start()
        }
        
        val frame = FrameLayout(this)
        frame.addView(button)
        setContentView(frame)
    }

    private fun start() {
        if (!Settings.canDrawOverlays(this)) {
            val intent = Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                Uri.parse("package:$packageName"))
            startActivity(intent)
        } else {
            startProjection()
        }
    }

    private fun startProjection() {
        val mgr = getSystemService(MEDIA_PROJECTION_SERVICE) as android.media.projection.MediaProjectionManager
        startActivityForResult(mgr.createScreenCaptureIntent(), projectionRequestCode)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        if (requestCode == projectionRequestCode && resultCode == Activity.RESULT_OK && data != null) {
            ScreenshotUtil.initProjection(resultCode, data, this)
            startService(Intent(this, FloatingWindowService::class.java))
            startActivity(Intent(this, SettingsActivity::class.java))
            finish()
        } else {
            Toast.makeText(this, "Permission screen capture ditolak", Toast.LENGTH_SHORT).show()
            finish()
        }
    }
}
