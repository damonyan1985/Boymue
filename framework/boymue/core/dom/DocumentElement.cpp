// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "DocumentElement.h"
#include "Document.h"
#include "Layout.h"
#include "BlockLayout.h"
#include "ImageLayout.h"
#include "TextLayout.h"
#include "InputLayout.h"

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

bool DocumentElement::isText() const {
    return false;
}

bool DocumentElement::isImage() const {
    return false;
}

bool DocumentElement::isTextField() const {
    return false;
}

bool DocumentElement::isView() const {
    return false;
}

bool DocumentElement::isButton() const {
    return false;
}

layout::Layout* DocumentElement::createLayout() {
    if (m_layout) {
        return m_layout;
    }
    
    // 根据Element类型创建对应的Layout，使用is*()函数判断类型
    if (isImage()) {
        m_layout = new layout::ImageLayout(this);
    } else if (isText()) {
        m_layout = new layout::TextLayout(this);
    } else if (isTextField()) {
        m_layout = new layout::InputLayout(this);
    } else if (isView() || isButton()) {
        m_layout = new layout::BlockLayout(this);
    } else {
        // 默认创建BlockLayout
        m_layout = new layout::BlockLayout(this);
    }
    
    return m_layout;
}
}
}
