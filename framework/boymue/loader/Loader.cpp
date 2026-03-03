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

// 辅助结构：用于构建 ArgumemntList
struct ArgumemntListBuilder {
    std::shared_ptr<std::vector<Argumemnt>> args_vec;
    
    ArgumemntListBuilder() : args_vec(std::make_shared<std::vector<Argumemnt>>()) {}
    
    // 添加 String 类型参数
    ArgumemntListBuilder& addString(const char* str) {
        Argumemnt arg;
        arg.arg_type = 2; // String 类型
        arg.arg_value.str_value = str;
        args_vec->push_back(arg);
        return *this;
    }
    
    // 添加 String 类型参数（String 类型）
    ArgumemntListBuilder& addString(const String& str) {
        return addString(str.c_str());
    }
    
    // 添加 i32 类型参数
    ArgumemntListBuilder& addI32(int32_t value) {
        Argumemnt arg;
        arg.arg_type = 1; // i32 类型
        arg.arg_value.i32_value = value;
        args_vec->push_back(arg);
        return *this;
    }
    
    // 添加 usize 类型参数
    ArgumemntListBuilder& addUsize(size_t value) {
        Argumemnt arg;
        arg.arg_type = 3; // usize 类型
        arg.arg_value.usize_value = value;
        args_vec->push_back(arg);
        return *this;
    }
    
    // 添加 uintptr_t 类型参数（转换为 usize）
    ArgumemntListBuilder& addUintptr(uintptr_t value) {
        return addUsize(static_cast<size_t>(value));
    }
    
    // 构建 ArgumemntList
    ArgumemntList build() {
        ArgumemntList arg_list;
        arg_list.arg_count = static_cast<int32_t>(args_vec->size());
        arg_list.arg_list = args_vec->data();
        return arg_list;
    }
    
    // 获取 args_vec 的 shared_ptr（用于保持生命周期）
    std::shared_ptr<std::vector<Argumemnt>> getArgsVec() const {
        return args_vec;
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
static void bmnetGetExtCallback(ArgumemntList result_args) {
    if (result_args.arg_count >= 2 && result_args.arg_list != nullptr) {
        // 第一个返回值：String (响应内容)
        const Argumemnt* arg0 = &result_args.arg_list[0];
        if (arg0->arg_type == 2 && arg0->arg_value.str_value != nullptr) {
            const char* response_data = arg0->arg_value.str_value;
            size_t response_len = strlen(response_data);
            
            // 第二个返回值：usize (ext 参数)
            const Argumemnt* arg1 = &result_args.arg_list[1];
            if (arg1->arg_type == 3) {
                uintptr_t ext = arg1->arg_value.usize_value;
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
        // 使用 ArgumemntListBuilder 构造参数列表
        ArgumemntListBuilder builder;
        ArgumemntList arg_list = builder
            .addString(url)                                    // args[0]: url (String)
            .addString(headers)                                // args[1]: headers (String)
            .addUintptr(reinterpret_cast<uintptr_t>(client))  // args[2]: ext (usize)
            .build();
        
        // 保持 args_vec 的生命周期，确保在异步回调期间参数有效
        auto args_vec = builder.getArgsVec();
        
        bmnet_get_ext_impl(arg_list, bmnetGetExtCallback);
        
        // args_vec 会在 lambda 结束时自动释放，但由于是异步操作，
        // bmnet_get_ext_impl 应该会复制字符串，所以这里是安全的
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
