//
//  Loader.cpp
//  core
//
//  Created by yanbo on 2022/9/30.
//

#include "Loader.h"
#include "bmnet_main.h"
#include <vector>
#include <cstring>
#include <memory>

namespace boymue {

// 辅助结构：用于构建 ArgumentList
struct ArgumentListBuilder {
    SharedPtr<Vector<Argument>> m_argsVec;
    
    ArgumentListBuilder() : m_argsVec(std::make_shared<Vector<Argument>>()) {}
    
    // 添加 String 类型参数
    ArgumentListBuilder& addString(const char* str) {
        Argument arg;
        arg.arg_type = BMNET_ARG_STRING;
        arg.arg_value.str.str_value = str;
        arg.arg_value.str.str_len = str ? std::strlen(str) : 0;
        m_argsVec->push_back(arg);
        return *this;
    }
    
    // 添加 String 类型参数（String 类型）
    ArgumentListBuilder& addString(const String& str) {
        Argument arg;
        arg.arg_type = BMNET_ARG_STRING;
        arg.arg_value.str.str_value = str.c_str();
        arg.arg_value.str.str_len = str.size();
        m_argsVec->push_back(arg);
        return *this;
    }
    
    // 添加 i32 类型参数
    ArgumentListBuilder& addI32(int32_t value) {
        Argument arg;
        arg.arg_type = BMNET_ARG_I32;
        arg.arg_value.i32_value = value;
        m_argsVec->push_back(arg);
        return *this;
    }
    
    // 添加 usize 类型参数
    ArgumentListBuilder& addUsize(size_t value) {
        Argument arg;
        arg.arg_type = BMNET_ARG_USIZE;
        arg.arg_value.usize_value = value;
        m_argsVec->push_back(arg);
        return *this;
    }
    
    // 添加 uintptr_t 类型参数（转换为 usize）
    ArgumentListBuilder& addUintptr(uintptr_t value) {
        return addUsize(static_cast<size_t>(value));
    }
    
    // 构建 ArgumentList
    ArgumentList build() {
        ArgumentList arg_list;
        arg_list.arg_count = static_cast<int32_t>(m_argsVec->size());
        arg_list.arg_list = m_argsVec->data();
        return arg_list;
    }
    
    // 获取 args_vec 的 shared_ptr（用于保持生命周期）
    std::shared_ptr<Vector<Argument>> getArgsVec() const {
        return m_argsVec;
    }
};

// 接受bmnet回调数据（用于post请求）
static void bmCallback(const uint8_t *data, size_t len, uintptr_t ext) {
    LoaderClient* client = reinterpret_cast<LoaderClient*>(ext);
    if (client) {
        client->onResultCallback(data, len);
    }
}

// 接受bmnet_get_ext_impl回调数据
static void bmnetGetExtCallback(ArgumentList result_args) {
    if (result_args.arg_count >= 2 && result_args.arg_list != nullptr) {
        // 第一个返回值：String (响应内容)
        const Argument* result = &result_args.arg_list[0];
        if (result->arg_type == BMNET_ARG_STRING && result->arg_value.str.str_value != nullptr) {
            const char* response_data = result->arg_value.str.str_value;
            size_t response_len = result->arg_value.str.str_len;
            
            // 第二个返回值：usize (ext 参数)
            const Argument* callback = &result_args.arg_list[1];
            if (callback->arg_type == BMNET_ARG_USIZE) {
                uintptr_t ext = callback->arg_value.usize_value;
                LoaderClient* client = reinterpret_cast<LoaderClient*>(ext);
                if (client) {
                    client->onResultCallback(reinterpret_cast<const uint8_t*>(response_data), response_len);
                }
            }
        }
    }
}

Loader::Loader()
    : m_executor(ThreadExecutor::createDefault("loader")) {}

Loader::~Loader() { delete m_executor; }

// get请求
void Loader::get(const String& url, const String& headers, LoaderClient* client) const {    
    m_executor->submitTask([url = std::move(url), 
                            headers = std::move(headers), 
                            client] {
        // 使用 ArgumentListBuilder 构造参数列表
        ArgumentListBuilder builder;
        ArgumentList arg_list = builder
            .addString(url)                                    // args[0]: url (String)
            .addString(headers)                                // args[1]: headers (String)
            .addUintptr(reinterpret_cast<uintptr_t>(client))  // args[2]: ext (usize)
            .build();
        bmnet_get_ext_impl(arg_list, bmnetGetExtCallback);
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
