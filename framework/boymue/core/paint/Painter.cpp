// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "Painter.h"

#include "BoxPainter.h"
#include "TextPainter.h"
#include "Layout.h"
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
    case layout::Layout::kLayoutButton:
        return new ButtonPainter(layout);
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
    const SkRect& pr = info.paintRect;
    if (pr.width() <= 0 || pr.height() <= 0) {
        return;
    }

    PaintContext* destCtx = info.context;
    SkCanvas* destCanvas = destCtx->canvas();

    const bool needsRepaint = m_layout && m_layout->needsRepaint(pr);

    if (needsRepaint) {
        SkPictureRecorder recorder;
        SkCanvas* recCanvas = recorder.beginRecording(pr.width(), pr.height());
        if (!recCanvas) {
            return;
        }
        // paintImpl 使用文档坐标下的 paintRect；录制成节点局部 (0,0)-(w,h) 便于回放时 translate
        recCanvas->translate(-pr.x(), -pr.y());

        RecordingPaintContext paintCtx(recCanvas);
        info.context = &paintCtx;
        paintImpl(info);
		info.context = destCtx;

        if (m_picture) {
            m_picture->unref();
            m_picture = nullptr;
        }
        m_picture = recorder.endRecording();
        if (m_layout) {
            m_layout->didRepaint(pr);
        }
    }

    destCanvas->save();
    destCanvas->translate(pr.x(), pr.y());
    destCanvas->drawPicture(m_picture);
    destCanvas->restore();
}
}  // namespace painter

}  // namespace boymue
