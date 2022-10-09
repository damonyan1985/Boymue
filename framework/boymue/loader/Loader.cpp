//
//  Loader.cpp
//  core
//
//  Created by yanbo on 2022/9/30.
//

#include "Loader.h"
#include "bmnet_main.h"

namespace boymue {
Loader::Loader()
    : m_executor(ThreadExecutor::createDefault("loader")) {}

void Loader::bmCallback(const uint8_t *data, size_t len, uintptr_t ext) {
    LoaderClient* client = reinterpret_cast<LoaderClient*>(ext);
    if (client) {
        client->onResultCallback(data, len);
    }
}

// get请求
void Loader::get(const String& url, const String& headers, LoaderClient* client) const {
    m_executor->submitTask([url = std::move(url),
                            headers = std::move(headers),
                            client = client] {
        bmnet_get(url.c_str(), headers.c_str(), bmCallback, (uintptr_t)client);
    });
}

// post请求
void Loader::post(const String& url, const String& headers, const String& data, LoaderClient* client) const {
    m_executor->submitTask([url = std::move(url),
                            headers = std::move(headers),
                            data = std::move(data),
                            client = client] {
        bmnet_post(url.c_str(), headers.c_str(), data.c_str(), bmCallback, (uintptr_t)client);
    });
}

}
