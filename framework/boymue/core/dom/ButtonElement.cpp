//
//  ButtonElement.cpp
//  core
//
//  Created by yanbo on 2022/9/6.
//

#include "ButtonElement.h"

namespace boymue {
namespace dom {
ButtonElement::ButtonElement(Document* dom)
    : DocumentElement(dom) {}

bool ButtonElement::isButton() const {
    return true;
}

String ButtonElement::tagName() const {
    return "button";
}
}
}
