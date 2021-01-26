// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#include "BoymueView.h"

namespace boymue {
BoymueView::BoymueView()
  : m_document(std::make_unique<Document>()) {
}

}  // namespace boymue