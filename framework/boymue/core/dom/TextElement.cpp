// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "TextElement.h"

namespace boymue {
namespace dom {
TextElement::TextElement(Document* dom, const String& text)
    : DocumentElement(dom), 
      m_text(text) {}

bool TextElement::isText() const {
  return true;
}

String TextElement::tagName() const {
  return "text";
}

const String& TextElement::text() const {
  return m_text;
}

}
}  // namespace boymue
