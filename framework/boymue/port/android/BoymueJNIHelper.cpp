#include "BoymueJNIHelper.h"

namespace boymue {
JavaVM* BoymueJNIHelper::s_javaVM = nullptr;
thread_local JNIEnv* BoymueJNIHelper::s_localEnv = nullptr;
JavaVM* BoymueJNIHelper::getJavaVM() {
    return s_javaVM;
}

void BoymueJNIHelper::setJavaVM(JavaVM* jvm) {
    s_javaVM = jvm;
}    

JNIEnv* BoymueJNIHelper::getEnv() {
    if (s_localEnv == nullptr)
        cacheEnv(s_javaVM);
    return s_localEnv;
}

JNIEnv* BoymueJNIHelper::cacheEnv(JavaVM* jvm) {
    JNIEnv* env = nullptr;
    // get jni environment
    jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

    switch (ret) {
    case JNI_OK:
        s_localEnv = env;
        return env;

    case JNI_EDETACHED:
        // Thread not attached

        // TODO : If calling AttachCurrentThread() on a native thread
        // must call DetachCurrentThread() in future.
        // see: http://developer.android.com/guide/practices/design/jni.html

        if (jvm->AttachCurrentThread(&env, nullptr) < 0) {
            return nullptr;
        } else {
            s_localEnv = env;
            return env;
        }

    case JNI_EVERSION:
    default:
        return nullptr;
    }
}
}

