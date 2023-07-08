package com.boymue.app.shell.vmodel

import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.boymue.app.shell.model.data.BoyiaUserData

class UserViewModel : ViewModel() {
    private val liveData = MutableLiveData<BoyiaUserData>()

    fun observe(owner: LifecycleOwner) {
        liveData.observe(owner) {

        }
    }
}