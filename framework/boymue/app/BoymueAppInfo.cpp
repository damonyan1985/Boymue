//
//  BoymueAppInfo.cpp
//  core
//
//  Created by yanbo on 2022/9/5.
//

#include "BoymueAppInfo.h"
#include "BoymueBridge.h"
#include "FileUtil.h"
#include "rapidjson/document.h"

namespace boymue {
BoymueAppInfo::BoymueAppInfo()
    : appId(0) {
    
}

void BoymueAppInfo::parseConfig() {
    String configContent = std::move(FileUtil::readFile(BoymueBridge::getSourcePath("/" + appName + "/app.json")));
    rapidjson::Document dom;
    dom.Parse(configContent.c_str(), configContent.size());
    
    printf("page name=%s\n", dom["pages"][0]["ui"].GetString());
}
}
