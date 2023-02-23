package com.boymue.app.shell.model.handler

import com.boymue.shell.app.proto.Base.ResponseData
import io.reactivex.Observable
import retrofit2.Response
import retrofit2.Retrofit

interface BaseHandler {
    fun doHandler(retrofit: Retrofit, params: Map<String, String>) : Observable<Response<ResponseData>>
}