#include "jni.h"
#include "BoymueJNIHelper.h"

int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* methods, int numMethods)
{
    jclass clazz = env->FindClass(className);
    if (!clazz) {
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, methods, numMethods) < 0) {
        env->DeleteLocalRef(clazz);
        return JNI_FALSE;
    }

    env->DeleteLocalRef(clazz);
    return JNI_TRUE;
}

extern int registerBoymueAppNatives(JNIEnv* env);

typedef int (*NativeMethod)(JNIEnv*);

struct NativeRegistration {
    NativeMethod method;
};

static NativeRegistration gBoyiaRegs[] = {
    { registerBoymueAppNatives },
    { NULL }
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    boymue::BoymueJNIHelper::setJavaVM(vm);

    JNIEnv* env = 0;
    jint result = -1;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    const NativeRegistration* reg = gBoyiaRegs;
    while (reg->method) {
        if (reg->method(env) < 0) {
            return result;
        }

        ++reg;
    }

    return JNI_VERSION_1_4;
}