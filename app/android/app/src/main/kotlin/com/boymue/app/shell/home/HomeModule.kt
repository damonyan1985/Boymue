package com.boymue.app.shell.home

import androidx.lifecycle.ViewModelProvider
import com.boymue.app.shell.model.data.BoyiaUserData
import com.boymue.app.shell.module.IModule
import com.boymue.app.shell.module.IModuleContext
import com.boymue.app.shell.vmodel.UserViewModel

class HomeModule: IModule {
    override fun init() {
    }

    override fun show(context: IModuleContext) {
        val userModel = ViewModelProvider(context.getActivity(), ViewModelProvider.NewInstanceFactory()).get(UserViewModel::class.java)
        val fragment = HomeFragment()

    }

    override fun hide() {
    }

    override fun dispose() {
    }

}