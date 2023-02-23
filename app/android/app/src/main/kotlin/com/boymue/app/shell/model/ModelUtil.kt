package com.boymue.app.shell.model

import com.boymue.app.shell.model.handler.BaseHandler
import com.boymue.app.shell.model.handler.LoginHandler
import com.boymue.app.shell.model.service.BaseService
import com.boymue.app.shell.model.service.LoginService
import com.boymue.app.shell.model.ssl.SSlHepler
import com.boymue.shell.app.proto.Base
import com.boymue.shell.app.proto.Base.ResponseData
import com.boymue.shell.app.proto.User.UserData
import com.google.protobuf.ExtensionRegistry
import com.google.protobuf.Any
import com.google.protobuf.Message
import io.reactivex.Observable
import io.reactivex.Observer
import okhttp3.OkHttpClient
import retrofit2.Retrofit
import retrofit2.adapter.rxjava.RxJavaCallAdapterFactory
import retrofit2.converter.protobuf.ProtoConverterFactory
import io.reactivex.functions.Function
import retrofit2.Response
import io.reactivex.Scheduler
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.Disposable
import io.reactivex.functions.Consumer
import io.reactivex.schedulers.Schedulers
import retrofit2.create
import java.time.Duration
import java.util.concurrent.ConcurrentHashMap

/**
 * Https请求封装
 */
object ModelUtil {
    const val TAG = "ModelUtil"
    const val BASE_URL = "https://127.0.0.1:8443"
    const val API_VERSION = "v1"
    const val LOGIN_URL: String = "user/${API_VERSION}/login"
    const val LOGOUT_URL: String = "user/${API_VERSION}/logout"

    val registry = ConcurrentHashMap<String, Class<out BaseHandler>>()

    fun init() {
        registry[UserData::class.toString()] = LoginHandler::class.java
    }

    fun createRetrofit(): Retrofit {
        val client = OkHttpClient.Builder()
                .connectTimeout(Duration.ofMillis(600))
                .readTimeout(Duration.ofMillis(600))
                .sslSocketFactory(SSlHepler.sslFactory())
                .hostnameVerifier { _, _ -> true }
                .addInterceptor { chain ->
                    chain.proceed(chain.request())
                            .newBuilder()
                            .header("User-Token", "")
                            .build()
                }
                .build()

        return Retrofit.Builder()
                .client(client)
                .baseUrl(BASE_URL)
                .addConverterFactory(ProtoConverterFactory.createWithRegistry(ExtensionRegistry.newInstance()))
                .addCallAdapterFactory(RxJavaCallAdapterFactory.create())
                .build()
    }

    inline fun <reified T: Message> request(params: Map<String, String>) {
        val retrofit = createRetrofit()
        val cls = registry[T::class.toString()]
        val observable = cls?.newInstance()?.doHandler(retrofit, params) ?: return
        observable.map(object : Function<Response<ResponseData>, T> {
            override fun apply(t: Response<ResponseData>): T? {
                val resp = t.body()
                return resp?.data?.unpack(T::class.java)
            }
        }).subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(object : Consumer<T> {
                    override fun accept(t: T?) {
                    }
                })
    }



    fun login(name: String, password: String) {
        val retrofit = createRetrofit()
        val service = retrofit.create(LoginService::class.java)
        val observable = service.login("","")
        observable.map(object : Function<Response<ResponseData>, UserData> {
            override fun apply(t: Response<ResponseData>): UserData? {
                val resp = t.body()
                return resp?.data?.unpack(UserData::class.java)
            }
        }).subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(object : Consumer<UserData> {
                    override fun accept(t: UserData?) {
                    }
                })
    }
}
