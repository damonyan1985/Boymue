package com.boymue.app.shell.model

import com.boymue.app.shell.model.handler.BaseHandler
import com.boymue.app.shell.model.handler.LoginHandler
import com.boymue.app.shell.model.ssl.SSlHepler
import com.boymue.shell.app.proto.Base.ResponseData
import com.boymue.shell.app.proto.User.UserData
import com.google.protobuf.ExtensionRegistry
import com.google.protobuf.Message
import okhttp3.OkHttpClient
import retrofit2.Retrofit
import retrofit2.converter.protobuf.ProtoConverterFactory
import io.reactivex.functions.Function
import retrofit2.Response
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.functions.Consumer
import io.reactivex.schedulers.Schedulers
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory
import java.time.Duration
import java.util.concurrent.ConcurrentHashMap

/**
 * Https请求封装
 */
object ModelUtil {
    const val TAG = "ModelUtil"
    private const val BASE_URL = "https://127.0.0.1:8443"
    private const val API_VERSION = "v1"
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
                //.addCallAdapterFactory(RxJavaCallAdapterFactory.create())
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
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
}
