//
//  JsRequireModuleApi.cpp
//  core
//
//  Created by yanbo on 2022/9/22.
//

#include "JsRequireModuleApi.h"
#include "FileUtil.h"
#include "BoymueBridge.h"
#include "BoymueApplication.h"

namespace boymue {
JsRequireModuleApi::JsRequireModuleApi(BoymueApplication* context)
    : JsApiInterface(context) {}

const char* JsRequireModuleApi::name() const { return "require"; }

void JsRequireModuleApi::execute(const String& params,
             JsApiCallback* callback) {
    if (m_scriptSet[params]) {
        return;
    }
    
    String srcPath = std::move(BoymueBridge::getSourcePath(params));
    String srcText = std::move(FileUtil::readFile(srcPath));
    
    context()->runtime()->evaluateJs(srcText, params);
    
    m_scriptSet[params] = true;
}
}
