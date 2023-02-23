package com.boymue.app.shell

import android.app.Application
import android.content.Context
import androidx.multidex.MultiDex

class BoymueApplication: Application() {
    companion object {
        private var INSTANCE : BoymueApplication? = null
        fun instance(): BoymueApplication? {
            return INSTANCE
        }
    }

    override fun attachBaseContext(base: Context?) {
        super.attachBaseContext(base)
        MultiDex.install(base)
    }

    override fun onCreate() {
        super.onCreate()
        INSTANCE = this
    }
}