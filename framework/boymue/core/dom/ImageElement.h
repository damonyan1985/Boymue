// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef ImageElement_h
#define ImageElement_h

#include "StringUtil.h"

#include "DocumentElement.h"

namespace boymue {
namespace dom {
class ImageElement : public DocumentElement {
 public:
  ImageElement(Document* dom);
  void parseAttribute(const char** atts) override;

  bool isImage() const override;
  String tagName() const override;
  const String& src() const { return m_url; }

 private:
  String m_url;
};
}
}  // namespace boymue
#endif  // !ImageElement_h
