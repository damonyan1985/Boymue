// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#include "BoymueView.h"
#include "BoymueApplication.h"

namespace boymue {
BoymueView::BoymueView(BoymueApplication* context)
  : m_context(context)
  , m_root(std::make_unique<Layout>()) {
}

}  // namespace boymue