// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef ViewElement_h
#define ViewElement_h

#include "DocumentElement.h"

namespace boymue {
namespace dom {
class ViewElement : public DocumentElement {
 public:
  ViewElement(Document* dom);
  virtual void parseAttribute(const char** atts);
  String tagName() const override;

  bool isView() const override;
};
}
}  // namespace boymue
#endif  // !ViewElement
