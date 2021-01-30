// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "Painter.h"

#include "SkPictureRecorder.h"

namespace boymue {
Painter::Painter() : m_picture(nullptr) {}

Painter::~Painter() {
  if (m_picture) {
    m_picture->unref();
  }
}

void Painter::paint(PaintInfo& info) {
  SkPictureRecorder recorder;
  SkCanvas* canvas = recorder.beginRecording(info.rect);
  paintImpl(canvas);
  m_picture = recorder.endRecording();
}
}  // namespace boymue