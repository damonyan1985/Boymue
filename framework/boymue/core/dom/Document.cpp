// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#include "Document.h"

#include "DomTags.h"
#include "ImageElement.h"
#include "ViewElement.h"
#include "ButtonElement.h"
#include "StringUtil.h"
#include "expat.h"

namespace boymue {
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
    static_cast<Document*>(dom)->getParseStack()->pop();
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

Document::Document()
    : m_root(nullptr) {}

void Document::initDocument(const std::string& content) {
  XML_Parser parser = XML_ParserCreate(NULL);
  // OnStartElement与OnEndElement都是XML_Parser中的属性
  XML_SetUserData(parser, this);
  XML_SetElementHandler(parser, &OnStartElement, &OnEndElement);
  {
      XML_SetCharacterDataHandler(parser, &OnCharacters);
  }

  XML_Parse(parser, content.c_str(), content.length(), 0);
  XML_ParserFree(parser);
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
    default:
      break;
  }

  if (parent && element) {
    parent->addChild(element);
  }

  return element;
}

void Document::createElement(int tag, int eid, int pid) {
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
      default:
        break;
    }
    
    if (element) {
        element->setElementId(eid);
    }
    
    if (pid != 0) {
        DocumentElement* parent = m_uniqueElems[pid];
        if (parent && element) {
            parent->addChild(element);
        }
    }
}

void Document::removeElement(int pid, int eid) {
    DocumentElement* parent = m_uniqueElems[pid];
    DocumentElement* child = m_uniqueElems[eid];
    
    if (parent && child) {
        parent->removeChild(child);
    }
}

Stack<DocumentElement*>* Document::getParseStack() {
  return &m_parseStack;
}

void Document::addUniqueElement(DocumentElement* elem) {
    m_uniqueElems.emplace(elem->elementId(), elem);
}

void Document::addStyleElement(DocumentElement* elem) {
    m_styleElems.emplace(elem->styleId(),  elem);
}

void Document::setElementProperty(int eid, const String& key, const String& value) {
    DocumentElement* elem = m_uniqueElems[eid];
    if (elem) {
        elem->setProperty(key, value);
    }
}
}  // namespace boymue
