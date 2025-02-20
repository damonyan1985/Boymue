//
//  JsRequestApi.cpp
//  core
//
//  Created by yanbo on 2022/10/9.
//

#include "JsRequestApi.h"
#include "BoymueApplication.h"
#include "rapidjson/document.h"
//#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
//#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"

namespace boymue {
class JsRequestClient : public LoaderClient {
public:
    JsRequestClient(JsApiCallback* callback, BoymueApplication* ctx)
        : m_callback(callback)
        , m_context(ctx) {}
    
    virtual void onResultCallback(const uint8_t* data, size_t len) override {
        // must copy data
        String result = String((const char*)data, len);
        RuntimeClosure task = [cb = m_callback, res = std::move(result)](JsRuntime* runtime) {
            cb->callback(res);
        };
        m_context->doRuntimeAction(task);
        
        delete this;
    }
    
private:
    JsApiCallback* m_callback;
    BoymueApplication* m_context;
};

const char* JsRequestApi::name() const { return "request"; }

JsRequestApi::JsRequestApi(BoymueApplication* context)
    : JsApiInterface(context) {}

void JsRequestApi::execute(const String& params,
                           JsApiCallback* callback) {
    rapidjson::Document json;
    json.Parse<0>(params.c_str());
    
    rapidjson::StringBuffer headers;
    rapidjson::Writer<rapidjson::StringBuffer> writer(headers);
    
    rapidjson::Value& headersValue = json["headers"];
    headersValue.Accept(writer);

    const char* method = json["method"].GetString();
    const char* url = json["url"].GetString();
    if (::strcmp(method, "GET") == 0) {
        context()->loader().get(url,
            headers.GetString(),
            new JsRequestClient(callback, context()));
    } else if (::strcmp(method, "POST") == 0) {
        const char* body = json["params"].GetString();
        context()->loader().post(url,
            headers.GetString(),
            body,
            new JsRequestClient(callback, context()));
    }
}

ThreadExecutor* JsRequestApi::executor() {
    return context()->ioExecutor();
}
}
