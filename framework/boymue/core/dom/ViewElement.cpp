// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "ViewElement.h"

namespace boymue {
namespace dom {
ViewElement::ViewElement(Document* dom)
    : DocumentElement(dom) {}

void ViewElement::parseAttribute(const char **atts) {
//    for (int i = 0; atts[i]; i+=2) {
//        String name = atts[i];
//        String value = atts[i+1];
//    }
}

bool ViewElement::isView() const {
  return true;
}

String ViewElement::tagName() const {
  return "view";
}
}
}
