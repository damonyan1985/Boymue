// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "ViewElement.h"
#include "StringUtil.h"

namespace boymue {
ViewElement::ViewElement(Document* dom)
    : DocumentElement(dom) {}

void ViewElement::parseAttribute(const char **atts) {
    for (int i = 0; atts[i]; i+=2) {
        String name = atts[i];
        String value = atts[i+1];
        
        std::hash<String> hash;
        
        printf("hash = %lx", hash(value));
        if (StringUtil::startWidth(name, kAppPrefix)) {
            Vector<String> props = StringUtil::split(name, kAttrSplitFlag);
            if (props.size() <= 1) {
                return;
            }
            
            // value去除双层大括号，完成属性绑定
            if (StringUtil::startWidth(value, "{")) {
                value = value.substr(2, value.size() - 4);
            }
            
            if (props[1] == "for") {
                
            } else if (props[1] == "if") {
                
            }
        }
    }
}

void ViewElement::addChild(DocumentElement* element) {
    m_children.push_back(SharedPtr<DocumentElement>(element));
}
}
