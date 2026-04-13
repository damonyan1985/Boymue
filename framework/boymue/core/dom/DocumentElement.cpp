// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "DocumentElement.h"

#include <cctype>

#include "Document.h"
#include "Layout.h"
#include "BlockLayout.h"
#include "ImageLayout.h"
#include "TextLayout.h"
#include "InputLayout.h"

namespace boymue {
namespace dom {

namespace {

String keyToLower(const String& key) {
    String k = key;
    for (char& c : k) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return k;
}

}  // namespace

DocumentElement::DocumentElement(Document* dom)
    : m_dom(dom)
    , m_uid(0) {}

DocumentElement::~DocumentElement() {
    if (!m_domElementId.empty() && m_dom) {
        m_dom->unregisterElementId(m_domElementId, this);
    }
}

void DocumentElement::addChild(DocumentElement* element) {
    if (!element) {
        return;
    }
    element->setParent(this);
    m_children.push_back(SharedPtr<DocumentElement>(element));
}

void DocumentElement::removeChild(DocumentElement* element) {
    if (!element) {
        return;
    }
    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        if (it->get() == element) {
            element->setParent(nullptr);
            m_children.erase(it);
            break;
        }
    }
}

void DocumentElement::visitChildren(
    const std::function<void(DocumentElement*)>& visitor) const {
    for (const auto& c : m_children) {
        if (c) {
            visitor(c.get());
        }
    }
}

layout::Layout* DocumentElement::layout() { return m_layout; }

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
    String kl = keyToLower(key);
    if (kl == "id") {
        if (!m_domElementId.empty() && m_dom) {
            m_dom->unregisterElementId(m_domElementId, this);
        }
        m_domElementId = value;
        if (!m_domElementId.empty() && m_dom) {
            m_dom->registerElementById(m_domElementId, this);
        }
    }
    m_properties[key] = value;
    for (DocumentElement* e = this; e; e = e->parent()) {
        if (layout::Layout* lay = e->layout()) {
            lay->invalidatePainter();
        }
    }
}

String DocumentElement::getProperty(const String& key) const {
    if (keyToLower(key) == "id") {
        return m_domElementId;
    }
    auto it = m_properties.find(key);
    return it == m_properties.end() ? String() : it->second;
}

String DocumentElement::tagName() const {
    return String();
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

bool DocumentElement::isForm() const {
    return false;
}

bool DocumentElement::isIframe() const {
    return false;
}

bool DocumentElement::isStyle() const {
    return false;
}

layout::Layout* DocumentElement::createLayout() {
    if (m_layout) {
        return m_layout;
    }

    if (isImage()) {
        m_layout = new layout::ImageLayout(this);
    } else if (isText()) {
        m_layout = new layout::TextLayout(this);
    } else if (isTextField()) {
        m_layout = new layout::InputLayout(this);
    } else if (isButton()) {
        m_layout = new layout::ButtonLayout(this);
    } else if (isStyle()) {
        return nullptr;
    } else if (isView() || isForm() || isIframe()) {
        m_layout = new layout::BlockLayout(this);
    } else {
        m_layout = new layout::BlockLayout(this);
    }

    return m_layout;
}

// --- IframeElement ---

String IframeElement::normalizeKey(const String& key) {
    return keyToLower(key);
}

void IframeElement::applyIframeAttribute(const String& keyLower, const String& value) {
    if (keyLower == "src") {
        m_src = value;
    } else if (keyLower == "name") {
        m_nameAttr = value;
    } else if (keyLower == "sandbox") {
        m_sandbox = value;
    }
}

IframeElement::IframeElement(Document* dom)
    : DocumentElement(dom) {}

void IframeElement::parseAttribute(const char** atts) {
    if (!atts) {
        return;
    }
    for (int i = 0; atts[i]; i += 2) {
        String k = atts[i];
        const char* av = atts[i + 1];
        String v = av ? String(av) : String();
        setProperty(k, v);
    }
}

void IframeElement::setProperty(const String& key, const String& value) {
    applyIframeAttribute(normalizeKey(key), value);
    DocumentElement::setProperty(key, value);
}

String IframeElement::getProperty(const String& key) const {
    String kl = normalizeKey(key);
    if (kl == "src") {
        return m_src;
    }
    if (kl == "name") {
        return m_nameAttr;
    }
    if (kl == "sandbox") {
        return m_sandbox;
    }
    return DocumentElement::getProperty(key);
}

bool IframeElement::isIframe() const {
    return true;
}

String IframeElement::tagName() const {
    return "iframe";
}

// --- StyleElement ---

StyleElement::StyleElement(Document* dom)
    : DocumentElement(dom) {}

String StyleElement::tagName() const {
    return "style";
}

bool StyleElement::isStyle() const {
    return true;
}

}  // namespace dom
}  // namespace boymue
