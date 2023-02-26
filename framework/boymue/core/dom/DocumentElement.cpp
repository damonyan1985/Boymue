// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "DocumentElement.h"
#include "Document.h"
#include "Layout.h"

namespace boymue {
namespace dom {
DocumentElement::DocumentElement(Document* dom)
    : m_dom(dom)
    , m_elemId(0) {}

void DocumentElement::addChild(DocumentElement* element) {}
layout::Layout* DocumentElement::layout() { return nullptr; }

void DocumentElement::setElementId(int elemId) {
    m_elemId = elemId;
    m_dom->addUniqueElement(this);
}

void DocumentElement::setStyleId(const String& styleId) {
    m_styleId = styleId;
    m_dom->addStyleElement(this);
}

int DocumentElement::elementId() const {
    return m_elemId;
}

const String& DocumentElement::styleId() const {
    return m_styleId;
}

void DocumentElement::setProperty(const String& key, const String& value) {
    m_properties[key] = value;
}
}
}
