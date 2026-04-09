// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "TextPainter.h"

#include "Layout.h"
#include "TextElement.h"
#include "SkCanvas.h"
#include "SkPaint.h"

namespace boymue {
namespace painter {

TextPainter::TextPainter(layout::Layout* layout)
    : Painter(layout) {}

void TextPainter::paintImpl(PaintInfo& info) {
    SkCanvas* canvas = info.context ? info.context->canvas() : nullptr;
    if (!canvas || !m_layout) {
        return;
    }
    auto* te = static_cast<dom::TextElement*>(m_layout->domElement());
    if (!te) {
        return;
    }
    const String& text = te->text();
    if (text.empty()) {
        return;
    }
    const css::Style& st = m_layout->style();
    SkPaint paint;
    paint.setAntiAlias(true);
    float fontPx = st.fontSizePx > 0 ? static_cast<float>(st.fontSizePx) : 14.f;
    paint.setTextSize(fontPx);
    paint.setColor(static_cast<SkColor>(st.color.value()));

    SkScalar baseline = info.paintRect.y() + fontPx * 0.85f;
    canvas->drawText(text.c_str(), text.size(), info.paintRect.x() + 2.f, baseline,
                     paint);
}

}  // namespace painter
}  // namespace boymue
