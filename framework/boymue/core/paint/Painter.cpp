// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "Painter.h"

#include "BoxPainter.h"
#include "TextPainter.h"
#include "Layout.h"
#include "ImageLayout.h"
#include "InputLayout.h"
#include "PaintContext.h"
#include "SkPictureRecorder.h"

namespace boymue {
namespace painter {

Painter* Painter::createPainter(layout::Layout* layout) {
    if (!layout) {
        return nullptr;
    }
    switch (layout->type()) {
    case layout::Layout::kLayoutBlock:
        return new BoxPainter(layout);
    case layout::Layout::kLayoutImage:
        return new ImagePainter(layout);
    case layout::Layout::kLayoutText:
        return new TextPainter(layout);
    case layout::Layout::kLayoutInput:
        return new InputPainter(layout);
    default:
        return nullptr;
    }
}

Painter::Painter(layout::Layout* layout)
    : m_picture(nullptr)
    , m_layout(layout) {}

Painter::~Painter() {
    if (m_picture) {
        m_picture->unref();
    }
}

void Painter::paint(PaintInfo& info) {
    if (!info.context || !info.context->canvas()) {
        return;
    }
    paintImpl(info);
}
}  // namespace painter

}  // namespace boymue
