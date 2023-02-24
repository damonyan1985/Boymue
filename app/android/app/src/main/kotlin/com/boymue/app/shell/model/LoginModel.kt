package com.boymue.app.shell.model

import com.boymue.shell.app.proto.User

object LoginModel {
    fun login(name: String, password: String) {
        val params = mapOf("name" to "", "password" to "")
        ModelUtil.request<User.UserData>(params)
    }
}