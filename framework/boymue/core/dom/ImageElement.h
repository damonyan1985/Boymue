// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef ImageElement_h
#define ImageElement_h

#include <string>

#include "DocumentElement.h"

namespace boymue {
class ImageElement : public DocumentElement {
 public:
  ImageElement();

 private:
  std::string m_url;
};
}  // namespace boymue
#endif  // !ImageElement_h
