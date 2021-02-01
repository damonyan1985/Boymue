// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#include "Document.h"

#include "DomTags.h"
#include "ImageElement.h"
#include "ViewElement.h"
#include "expat.h"

namespace boymue {
// expat淡疼的一点是在多线程中调用会有问题，后期可以用threadlocal改造一下这个开源库
// 或者部分重写
// 处理标签开始
static void XMLCALL OnStartElement(void* dom, const char* name,
                                   const char** atts) {
  Document* document = static_cast<Document*>(dom);
  int tag = DomTags::instance()->getTag(name);
  std::stack<DocumentElement*>* stack = document->getParseStack();
  stack->push(document->createElement(tag, atts, stack->top()));
}

// 处理标签结束
static void XMLCALL OnEndElement(void* dom, const char* name) {
  static_cast<Document*>(dom)->getParseStack()->pop();
}

// 处理文本
static void XMLCALL OnCharacters(void* dom, const char* text, int len) {
  TextElement* element = new TextElement(text);
  DocumentElement* parent = static_cast<Document*>(dom)->getParseStack()->top();
  if (parent) {
    parent->addChild(element);
  }
}

void Document::initDocument(const std::string& content) {
  XML_Parser parser = XML_ParserCreate(NULL);
  XML_SetUserData(parser, this);
  XML_SetElementHandler(parser, &OnStartElement, &OnEndElement);
  { XML_SetCharacterDataHandler(parser, &OnCharacters); }

  XML_Parse(parser, content.c_str(), content.length(), 0);
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
    default:
      break;
  }

  if (parent) {
    parent->addChild(element);
  }

  return nullptr;
}

std::stack<DocumentElement*>* Document::getParseStack() {
  return &m_parseStack;
}
}  // namespace boymue