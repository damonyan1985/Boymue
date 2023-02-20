package com.boymue.app

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity

class BoymueHomeActivity: AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val intent = Intent()
        intent.action = "com.boymue.app1.action"
        intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK
        startActivity(intent)
    }
}