package com.boymue.app.shell.model.handler

import com.boymue.app.shell.model.service.BaseService
import com.boymue.app.shell.model.service.LoginService
import com.boymue.shell.app.proto.Base
import com.boymue.shell.app.proto.User.UserData
import io.reactivex.Observable
import retrofit2.Response
import retrofit2.Retrofit

class LoginHandler : BaseHandler {
    override fun doHandler(retrofit: Retrofit, params: Map<String, String>) : Observable<Response<Base.ResponseData>> {
        return retrofit.create(LoginService::class.java).login(params["name"]!!, params["password"]!!)
    }
}