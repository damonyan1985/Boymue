// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef TextElement_h
#define TextElement_h

#include <string>

#include "DocumentElement.h"

namespace boymue {
namespace dom {
class TextElement : public DocumentElement {
 public:
  TextElement(Document* dom, const String& text);
  bool isText() const override;
  String tagName() const override;
  const String& text() const;

 private:
  String m_text;
};
}
}  // namespace boymue
#endif  // !TextElement
