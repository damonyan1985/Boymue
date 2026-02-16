// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "Painter.h"
#include "BoxPainter.h"
#include "TextPainter.h"
#include "Layout.h"
#include "ImageLayout.h"
#include "SkPictureRecorder.h"

namespace boymue {
namespace painter {
class ImagePainter : public Painter {
public:
	ImagePainter(layout::Layout* layout) : Painter(layout) {}

	virtual void paintImpl(SkCanvas* canvas, PaintInfo& info) {
		layout::ImageLayout* layout = static_cast<layout::ImageLayout*>(m_layout);

		SkPaint paint;
		info.context->canvas()->drawBitmapRect(
			layout->image()->bitmap(),
			SkRect::MakeXYWH(layout->left(), layout->top(), layout->width(), layout->height()),
			&paint);
	}
};

Painter* Painter::createPainter(layout::Layout* layout) {
	switch (layout->type())
	{
	case layout::Layout::kLayoutBlock:
		return new BoxPainter(layout);
	case layout::Layout::kLayoutImage:
		return new ImagePainter(layout);
	case layout::Layout::kLayoutText:
		return new TextPainter(layout);
	default:
		return nullptr;
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