// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "DocumentElement.h"
#include "Document.h"
#include "Layout.h"

namespace boymue {
namespace dom {
DocumentElement::DocumentElement(Document* dom)
    : m_dom(dom)
    , m_uid(0) {}

void DocumentElement::addChild(DocumentElement* element) {}
layout::Layout* DocumentElement::layout() { return nullptr; }

void DocumentElement::setUid(int uid) {
    m_uid = uid;
    m_dom->addUniqueElement(this);
}

void DocumentElement::setStyleId(const String& styleId) {
    m_styleId = styleId;
    m_dom->addStyleElement(this);
}

int DocumentElement::uid() const {
    return m_uid;
}

const String& DocumentElement::styleId() const {
    return m_styleId;
}

void DocumentElement::setProperty(const String& key, const String& value) {
    m_properties[key] = value;
}
}
}
