// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef TextElement_h
#define TextElement_h

#include <string>

#include "DocumentElement.h"

namespace boymue {
class TextElement : public DocumentElement {
 public:
  TextElement(const std::string& text);

 private:
  std::string m_text;
};
}  // namespace boymue
#endif  // !TextElement
