package com.boymue.app.shell.module

import androidx.appcompat.app.AppCompatActivity
import com.boymue.app.shell.api.IPickImageLoader

/**
 * 模块上下文
 */
interface IModuleContext {
    fun rootId(): Int
    fun getActivity(): AppCompatActivity
    fun pickImage(loader: IPickImageLoader) {}
}