// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#ifndef BoymueView_h
#define BoymueView_h

#include "Document.h"
#include <memory>

namespace boymue {
class BoymueView {
 public:
  BoymueView();

 private:
  std::unique_ptr<Document> m_document;
};
}  // namespace boymue
#endif
