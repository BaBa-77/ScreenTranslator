package com.example.screentranslator

import android.os.Handler
import android.os.Looper
import okhttp3.*
import org.json.JSONObject
import java.io.IOException

object Translator {
    fun translate(text: String, source: String, target: String, callback: (String) -> Unit) {
        val client = OkHttpClient()
        val json = JSONObject().apply {
            put("q", text)
            put("source", source)
            put("target", target)
            put("format", "text")
        }

        val body = RequestBody.create("application/json".toMediaTypeOrNull(), json.toString())
        val request = Request.Builder()
            .url("https://libretranslate.de/translate")
            .post(body)
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onResponse(call: Call, response: Response) {
                val responseBody = response.body?.string()
                val translated = JSONObject(responseBody ?: "{}").optString("translatedText", "")
                Handler(Looper.getMainLooper()).post { callback(translated) }
            }

            override fun onFailure(call: Call, e: IOException) {
                callback("Terjemahan gagal: ${e.localizedMessage}")
            }
        })
    }
}