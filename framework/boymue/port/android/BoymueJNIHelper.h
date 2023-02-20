#ifndef BoymueJNIHelper_h
#define BoymueJNIHelper_h

#include <jni.h>
#include <thread>

namespace boymue {
class BoymueJNIHelper {
public:
    static void setJavaVM(JavaVM* jvm);
    static JavaVM* getJavaVM();
    // 不同线程使用不同的JNIEnv
    static JNIEnv* getEnv();

private:
    static JNIEnv* cacheEnv(JavaVM* jvm);  
    static JavaVM* s_javaVM; 
    thread_local static JNIEnv* s_localEnv;    
};

}

#endif