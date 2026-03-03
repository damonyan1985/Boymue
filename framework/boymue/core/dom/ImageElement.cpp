// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "ImageElement.h"

namespace boymue {
namespace dom {
ImageElement::ImageElement(Document* dom)
    : DocumentElement(dom) {}

bool ImageElement::isImage() const {
  return true;
}
}
}  // namespace boymue
