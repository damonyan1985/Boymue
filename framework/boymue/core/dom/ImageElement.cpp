// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "ImageElement.h"

#include <cctype>

namespace boymue {
namespace dom {
ImageElement::ImageElement(Document* dom)
    : DocumentElement(dom) {}

void ImageElement::parseAttribute(const char** atts) {
    if (!atts) {
        return;
    }
    for (int i = 0; atts[i]; i += 2) {
        String k = atts[i];
        const char* av = atts[i + 1];
        String v = av ? String(av) : String();
        for (char& c : k) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        if (k == "src") {
            m_url = v;
        }
        setProperty(atts[i], v);
    }
}

bool ImageElement::isImage() const {
  return true;
}

String ImageElement::tagName() const {
  return "img";
}
}
}  // namespace boymue
