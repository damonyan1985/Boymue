// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef TextElement_h
#define TextElement_h

#include <string>

#include "DocumentElement.h"

namespace boymue {
namespace layout {
class TextLayout;  
}  
namespace dom {
class TextElement : public DocumentElement {
 public:
  TextElement(Document* dom, const String& text);

 private:
  const String& text() const; 

  String m_text;
  friend class layout::TextLayout;
};
}
}  // namespace boymue
#endif  // !TextElement
