// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#ifndef BoymueView_h
#define BoymueView_h

#include "Document.h"
#include <memory>

namespace boymue {
class BoymueApplication;
class BoymueView {
 public:
  BoymueView(BoymueApplication* context);

 private:
  BoymueApplication* m_context;
  std::unique_ptr<Document> m_document;
};
}  // namespace boymue
#endif
