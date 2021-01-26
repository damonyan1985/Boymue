// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#ifndef Document_h
#define Document_h

#include <stack>
#include <string>

#include "DocumentElement.h"
#include "TextElement.h"

namespace boymue {
class Document {
 public:
  // 利用XML内容来初始化document
  void initDocument(const std::string& content);
  std::stack<DocumentElement*>* getParseStack();
  DocumentElement* createElement(int tag, const char** atts,
                                 DocumentElement* parent);

 private:
  std::stack<DocumentElement*> m_parseStack;
};
}  // namespace boymue
#endif  // !Document_h
