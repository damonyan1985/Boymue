// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#ifndef BoymueView_h
#define BoymueView_h

#include "Layout.h"
#include <memory>

namespace boymue {
class BoymueApplication;
class BoymueView {
 public:
  BoymueView(BoymueApplication* context);

 private:
  BoymueApplication* m_context;
  std::unique_ptr<Layout> m_root;
};
}  // namespace boymue
#endif
