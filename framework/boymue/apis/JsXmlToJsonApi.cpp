//
//  JsXmlToJsonApi.cpp
//  core
//
//  Created by yanbo on 2022/9/20.
//

#include "JsXmlToJsonApi.h"
#include "BoymueApplication.h"
#include "xml2json.h"
#include "FileUtil.h"
#include "BoymueBridge.h"

namespace boymue {

const char* JsXmlToJsonApi::name() const { return "xmlToJson"; }

JsXmlToJsonApi::JsXmlToJsonApi(BoymueApplication* context)
    : JsApiInterface(context) {
}

// 使用io thread进行解析
ThreadExecutor* JsXmlToJsonApi::executor() {
    return context()->ioExecutor();
}

// 将xml转换成虚拟dom的配置信息
void JsXmlToJsonApi::execute(const String& params,
                             JsApiCallback* callback) {
    if (!callback) {
        return;
    }
    
    String xmlPath = std::move(BoymueBridge::getSourcePath(params));
    String xmlText = std::move(FileUtil::readFile(xmlPath));
    if (xmlPath.empty()) {
        return;
    }
    
    callback->callback(xml2json(xmlText.c_str()));
}
}
