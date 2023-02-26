// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "Painter.h"
#include "BoxPainter.h"
#include "Layout.h"
#include "SkPictureRecorder.h"

namespace boymue {
namespace painter {
Painter* Painter::createPainter(layout::Layout* layout) {
	switch (layout->type())
	{
	case layout::Layout::kLayoutBlock:
		return new BoxPainter(layout);
	default:
		nullptr;
	}
}

Painter::Painter(layout::Layout *layout)
  : m_picture(nullptr)
  , m_layout(layout) {}

Painter::~Painter() {
  if (m_picture) {
    m_picture->unref();
  }
}

SkPicture* Painter::picture() const {
	return m_picture;
}

void Painter::paint(PaintInfo& info) {
  SkPictureRecorder recorder;
  SkCanvas* canvas = recorder.beginRecording(info.clipRect);
  paintImpl(canvas, info);
  m_picture = recorder.endRecording();
}
}  // namespace painter

}  // namespace boymue