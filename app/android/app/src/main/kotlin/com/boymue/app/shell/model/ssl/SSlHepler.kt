package com.boymue.app.shell.model.ssl

import com.boymue.app.shell.BoymueApplication
import java.security.KeyStore
import java.security.SecureRandom
import java.security.cert.CertificateFactory
import java.security.cert.X509Certificate
import javax.net.ssl.SSLContext
import javax.net.ssl.SSLSocketFactory
import javax.net.ssl.TrustManagerFactory
import javax.net.ssl.X509TrustManager

object SSlHepler {
    const val CERT_PATH = "cert"
    const val KEY_STORE_PASSWORD = ""
    const val KEY_STORE_TRUST_PASSWORD = ""

    private fun getCerts() : TrustManagerFactory? {
        try {
            val assets = BoymueApplication.instance()?.assets
            val certs = assets?.list(CERT_PATH)
            if (certs?.size == 0) {
                return null
            }

            val keyStore = KeyStore.getInstance(KeyStore.getDefaultType())
            keyStore.load(null)
            val factory = CertificateFactory.getInstance("X.509")
            for (i in 0 until certs!!.size) {
                val input = assets.open(certs[i])
                keyStore.setCertificateEntry((i + 1).toString(), factory.generateCertificate(input))
                input.close()
            }

            val tmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm())
            tmf.init(keyStore)
            return tmf
        } catch (e: Exception) {
            e.printStackTrace()
        }

        return null
    }

    fun sslFactory(): SSLSocketFactory? {
        try {
            val ctx = SSLContext.getInstance("TLS")
            val certs = getCerts()
            val trustManagers = if (certs != null) {
                certs.trustManagers as Array<X509TrustManager>
            } else {
                val manager = object : X509TrustManager {
                    override fun checkClientTrusted(chain: Array<out X509Certificate>?, authType: String?) {
                    }

                    override fun checkServerTrusted(chain: Array<out X509Certificate>?, authType: String?) {
                    }

                    override fun getAcceptedIssuers(): Array<X509Certificate> {
                        return arrayOf()
                    }
                }

                arrayOf(manager)
            }

            ctx.init(null, trustManagers, SecureRandom())
            return ctx.socketFactory
        } catch (e: Exception) {
            e.printStackTrace()
        }

        return null
    }
}