// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "DocumentElement.h"
#include "Document.h"
#include "Layout.h"

namespace boymue {
DocumentElement::DocumentElement(Document* dom)
    : m_dom(dom) {}

void DocumentElement::addChild(DocumentElement* element) {}
Layout* DocumentElement::layout() { return nullptr; }

}
