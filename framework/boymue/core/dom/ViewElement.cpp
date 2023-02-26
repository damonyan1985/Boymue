// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "ViewElement.h"
#include "StringUtil.h"

namespace boymue {
namespace dom {
ViewElement::ViewElement(Document* dom)
    : DocumentElement(dom) {}

void ViewElement::parseAttribute(const char **atts) {
//    for (int i = 0; atts[i]; i+=2) {
//        String name = atts[i];
//        String value = atts[i+1];
//    }
}

void ViewElement::addChild(DocumentElement* element) {
    m_children.push_back(SharedPtr<DocumentElement>(element));
}

void ViewElement::removeChild(DocumentElement* element) {
    m_children.remove(SharedPtr<DocumentElement>(element));
}
}
}
