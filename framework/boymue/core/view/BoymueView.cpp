// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#include "BoymueView.h"

namespace boymue {
BoymueView::BoymueView() { m_uiThread.start(); }

TaskRunner& BoymueView::getTaskRunner() const {
  return m_uiThread.getTaskRunner();
}
}  // namespace boymue