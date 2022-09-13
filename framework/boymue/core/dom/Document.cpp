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
  stack->push(document->createElement(tag, atts, stack->size() ? stack->top() : nullptr));
}

// 处理标签结束
static void XMLCALL OnEndElement(void* dom, const char* name) {
  static_cast<Document*>(dom)->getParseStack()->pop();
}

// 处理文本
static void XMLCALL OnCharacters(void* dom, const char* text, int len) {
  // 换行，以及空格都会返回，所以需要判断是否是空白文本
  if (StringUtil::isSpace(text, len)) {
    return;
  }
  TextElement* element = new TextElement(String(text, len));
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
      element = new ViewElement();
      // 如果Dom root不存在，则设置root
      if (!m_root) {
        m_root = element;
      }
      break;
    case DomTags::kImage:
      element = new ImageElement();
      break;
    case DomTags::kButton:
      element = new ButtonElement();
      break;
    default:
      break;
  }

  if (parent && element) {
    parent->addChild(element);
  }

  return element;
}

std::stack<DocumentElement*>* Document::getParseStack() {
  return &m_parseStack;
}
}  // namespace boymue
