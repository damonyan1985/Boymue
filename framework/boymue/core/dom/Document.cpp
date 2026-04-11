// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#include "Document.h"

#include <functional>

#include "DomTags.h"
#include "ImageElement.h"
#include "ViewElement.h"
#include "TextElement.h"
#include "ButtonElement.h"
#include "InputElement.h"
#include "StringUtil.h"
#include "expat.h"

namespace boymue {
namespace dom {
// 处理标签开始
static void XMLCALL OnStartElement(void* dom, const char* name,
                                   const char** atts) {
    Document* document = static_cast<Document*>(dom);
    int tag = DomTags::instance()->getTag(name);

    std::stack<DocumentElement*>* stack = document->getParseStack();
    DocumentElement* elem = document->createElement(tag, atts, stack->size() ? stack->top() : nullptr);
    stack->push(elem);

    elem->parseAttribute(atts);
}

// 处理标签结束
static void XMLCALL OnEndElement(void* dom, const char* name) {
    Document* document = static_cast<Document*>(dom);
    Stack<DocumentElement*>* stack = document->getParseStack();
    if (stack->empty()) {
        return;
    }
    DocumentElement* closed = stack->top();
    stack->pop();
    if (!closed) {
        return;
    }
    if (DomTags::instance()->getTag(name) == DomTags::kStyle && closed->isStyle()) {
        String css;
        closed->visitChildren([&](DocumentElement* c) {
            if (c && c->isText()) {
                css += static_cast<TextElement*>(c)->text();
            }
        });
        document->styleEngine().parseCSS(css);
    }
}

// 处理文本
static void XMLCALL OnCharacters(void* dom, const char* text, int len) {
    // 换行，以及空格都会返回，所以需要判断是否是空白文本
    if (StringUtil::isspace(text, len)) {
        return;
    }
    Document* document = static_cast<Document*>(dom);
    
    TextElement* element = new TextElement(document, String(text, len));
    DocumentElement* parent = static_cast<Document*>(dom)->getParseStack()->top();
    if (parent) {
        parent->addChild(element);
    }
}

Frame::Frame(Document* owner)
    : m_owner(owner) {}

void Frame::setViewport(LayoutUnit w, LayoutUnit h) {
    m_viewportWidth = w;
    m_viewportHeight = h;
}

Document::Document()
    : m_frame(this)
    , m_root(nullptr) {}

void Document::parseFromXML(const String& content) {
  XML_Parser parser = XML_ParserCreate(NULL);
  // OnStartElement与OnEndElement都是XML_Parser中的属性
  XML_SetUserData(parser, this);
  XML_SetElementHandler(parser, &OnStartElement, &OnEndElement);
  {
      XML_SetCharacterDataHandler(parser, &OnCharacters);
  }

  XML_Parse(parser, content.c_str(), content.length(), 0);
  XML_ParserFree(parser);
  relinkFormControls();
  if (m_root) {
    m_styleEngine.apply(m_root);
  }
}

DocumentElement* Document::createElement(int tag, const char** atts,
                                         DocumentElement* parent) {
  DocumentElement* element = nullptr;
  switch (tag) {
    case DomTags::kView:
      element = new ViewElement(this);
      // 如果Dom root不存在，则设置root
      if (!m_root) {
        m_root = element;
      }
      break;
    case DomTags::kImage:
      element = new ImageElement(this);
      break;
    case DomTags::kButton:
      element = new ButtonElement(this);
      break;
    case DomTags::kInput:
      element = new InputElement(this);
      break;
    case DomTags::kForm:
      element = new FormElement(this);
      break;
    case DomTags::kIframe:
      element = new IframeElement(this);
      break;
    case DomTags::kStyle:
      element = new StyleElement(this);
      break;
    default:
      break;
  }

  if (parent && element) {
    parent->addChild(element);
  }

  return element;
}

void Document::createElement(int tag, int uid, int pid) {
    DocumentElement* element = nullptr;
    switch (tag) {
      case DomTags::kView:
        element = new ViewElement(this);
        // 如果Dom root不存在，则设置root
        if (!m_root) {
          m_root = element;
        }
        break;
      case DomTags::kImage:
        element = new ImageElement(this);
        break;
      case DomTags::kButton:
        element = new ButtonElement(this);
        break;
      case DomTags::kInput:
        element = new InputElement(this);
        break;
      case DomTags::kForm:
        element = new FormElement(this);
        break;
      case DomTags::kIframe:
        element = new IframeElement(this);
        break;
      case DomTags::kStyle:
        element = new StyleElement(this);
        break;
      default:
        break;
    }
    
    if (element) {
        element->setUid(uid);
    }
    
    if (pid != 0) {
        DocumentElement* parent = m_uniqueElems[pid];
        if (parent && element) {
            parent->addChild(element);
        }
    }
}

void Document::removeElement(int pid, int uid) {
    DocumentElement* parent = m_uniqueElems[pid];
    DocumentElement* child = m_uniqueElems[uid];
    
    if (parent && child) {
        parent->removeChild(child);
    }
}

Stack<DocumentElement*>* Document::getParseStack() {
  return &m_parseStack;
}

void Document::addUniqueElement(DocumentElement* elem) {
    m_uniqueElems.emplace(elem->uid(), elem);
}

void Document::addStyleElement(DocumentElement* elem) {
    m_styleElems.emplace(elem->styleId(),  elem);
}

void Document::setElementProperty(int uid, const String& key, const String& value) {
    DocumentElement* elem = m_uniqueElems[uid];
    if (elem) {
        elem->setProperty(key, value);
        if (elem->isTextField()) {
            static_cast<InputElement*>(elem)->relinkFormOwner();
        }
    }
}

void Document::registerElementById(const String& id, DocumentElement* el) {
    if (id.empty() || !el) {
        return;
    }
    m_idElems[id] = el;
}

void Document::unregisterElementId(const String& id, DocumentElement* el) {
    if (id.empty()) {
        return;
    }
    auto it = m_idElems.find(id);
    if (it != m_idElems.end() && it->second == el) {
        m_idElems.erase(it);
    }
}

DocumentElement* Document::elementById(const String& id) const {
    if (id.empty()) {
        return nullptr;
    }
    auto it = m_idElems.find(id);
    return it == m_idElems.end() ? nullptr : it->second;
}

void Document::relinkFormControls() {
    if (!m_root) {
        return;
    }
    std::function<void(DocumentElement*)> walk;
    walk = [&](DocumentElement* el) {
        if (!el) {
            return;
        }
        if (el->isTextField()) {
            static_cast<InputElement*>(el)->relinkFormOwner();
        }
        el->visitChildren([&](DocumentElement* c) { walk(c); });
    };
    walk(m_root);
}
}
}  // namespace boymue
