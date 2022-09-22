//
//  JsUIOperationApi.cpp
//  core
//
//  Created by yanbo on 2022/9/19.
//

#include "JsUIOperationApi.h"
#include "Document.h"
#include "DomTags.h"
#include "rapidjson/document.h"

namespace boymue {
const char* JsUIOperationApi::name() const { return "operation"; }

JsUIOperationApi::JsUIOperationApi(BoymueApplication* context)
    : JsApiInterface(context) {
}

void JsUIOperationApi::execute(const String& params,
                               JsApiCallback* callback) {
    rapidjson::Document cmds;
    cmds.Parse(params.c_str());
    
    /// 命令是json数组组装的
    if (!cmds.IsArray()) {
        return;
    }
    
    Document* dom = new Document();
    for (rapidjson::SizeType i = 0; i < cmds.Size(); i++) {
        if (!cmds[i]["type"].IsInt()) {
            return;
        }
        
        int type = cmds[i]["type"].GetInt();
        switch (type) {
            case Document::kAddNode: {
                dom->createElement(type, cmds[i]["id"].GetInt(), cmds[i]["pid"].GetInt());
            } break;
            case Document::kSetProp: {
                dom->setElementProperty(cmds[i]["id"].GetInt(),
                                        cmds[i]["key"].GetString(),
                                        cmds[i]["value"].GetString());
            } break;
            case Document::kRemoveNode: {
                dom->removeElement(cmds[i]["pid"].GetInt(), cmds[i]["id"].GetInt());
            } break;
            default:
                break;
        }
    }
}
}