//
//  Loader.h
//  core
//
//  Created by yanbo on 2022/9/30.
//

#ifndef Loader_h
#define Loader_h

#include "StringUtil.h"
#include "ThreadExecutor.h"

namespace boymue {
class LoaderClient {
public:
    // 只返回结果
    virtual void onResultCallback(const uint8_t *data, size_t len) = 0;
};

class Loader {
public:
    // 接受bmnet回调数据
    static void bmCallback(const uint8_t *data, size_t len, uintptr_t ext);
    
    Loader();
    // get请求
    void get(const String& url, const String& headers, LoaderClient* client);
    // post请求
    void post(const String& url, const String& headers, const String& data, LoaderClient* client);
    
private:
    ThreadExecutor* m_executor;
};
}

#endif /* Loader_h */
