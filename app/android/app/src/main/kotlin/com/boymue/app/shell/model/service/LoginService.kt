package com.boymue.app.shell.model.service

import com.boymue.app.shell.model.ModelUtil
import com.boymue.shell.app.proto.Base.ResponseData
import io.reactivex.Observable
import retrofit2.Response
import retrofit2.http.Field
import retrofit2.http.POST
//import rx.Observable

interface LoginService {
    @POST(ModelUtil.LOGIN_URL)
    fun login(@Field("name") name: String, @Field("pwd") password: String) : Observable<Response<ResponseData>>

    @POST(ModelUtil.LOGOUT_URL)
    fun logout()
}