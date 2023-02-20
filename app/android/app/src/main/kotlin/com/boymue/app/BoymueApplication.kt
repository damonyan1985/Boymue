package com.boymue.app

import android.app.Application
import android.content.Context
import androidx.multidex.MultiDex
import leakcanary.AppWatcher

class BoymueApplication: Application() {
    override fun attachBaseContext(base: Context?) {
        super.attachBaseContext(base)
        MultiDex.install(base)
    }
}