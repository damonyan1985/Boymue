#include "BoxPainter.h"

#include <algorithm>

#include "Layout.h"
#include "Image.h"
#include "ImageLayout.h"
#include "TextFieldElement.h"
#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkRRect.h"
#include "SkRect.h"

namespace boymue {
namespace painter {

static SkColor skColorFromStyleColor(const css::Color& c) {
    return static_cast<SkColor>(c.value());
}

BoxPainter::BoxPainter(layout::Layout* layout)
    : Painter(layout) {}

void BoxPainter::paintImpl(PaintInfo& info) {
    SkCanvas* canvas = info.context ? info.context->canvas() : nullptr;
    if (!canvas || !m_layout) {
        return;
    }
    const css::Style& st = m_layout->style();
    SkRect box = info.paintRect;

    SkRRect rr;
    if (st.borderRadius > 0.f) {
        SkVector radii[4] = {
            {st.borderRadius, st.borderRadius},
            {st.borderRadius, st.borderRadius},
            {st.borderRadius, st.borderRadius},
            {st.borderRadius, st.borderRadius},
        };
        rr.setRectRadii(box, radii);
    }

    SkPaint fill;
    fill.setStyle(SkPaint::kFill_Style);
    fill.setColor(skColorFromStyleColor(st.bgColor));
    fill.setAntiAlias(true);
    if (st.borderRadius > 0.f) {
        canvas->drawRRect(rr, fill);
    } else {
        canvas->drawRect(box, fill);
    }

    LayoutUnit bw = st.borderTopWidth + st.borderLeftWidth;  // 简化：四边取可见宽度
    if (bw > 0.f || st.borderRightWidth > 0.f || st.borderBottomWidth > 0.f) {
        SkPaint border;
        border.setStyle(SkPaint::kStroke_Style);
        border.setColor(skColorFromStyleColor(st.borderColor));
        border.setStrokeWidth(std::max({st.borderTopWidth, st.borderRightWidth,
                                        st.borderBottomWidth, st.borderLeftWidth}));
        border.setAntiAlias(true);
        if (st.borderRadius > 0.f) {
            canvas->drawRRect(rr, border);
        } else {
            canvas->drawRect(box, border);
        }
    }
}

ImagePainter::ImagePainter(layout::Layout* layout)
    : Painter(layout) {}

void ImagePainter::paintImpl(PaintInfo& info) {
    SkCanvas* canvas = info.context ? info.context->canvas() : nullptr;
    if (!canvas || !m_layout) {
        return;
    }
    auto* imgLay = static_cast<layout::ImageLayout*>(m_layout);
    Image* img = imgLay->image();
    if (!img || img->bitmap().empty()) {
        SkPaint p;
        p.setColor(SK_ColorLTGRAY);
        canvas->drawRect(info.paintRect, p);
        return;
    }
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setFilterQuality(kLow_SkFilterQuality);
    SkRect dst = info.paintRect;
    canvas->drawBitmapRect(img->bitmap(), dst, &paint);
}

InputPainter::InputPainter(layout::Layout* layout)
    : Painter(layout) {}

void InputPainter::paintImpl(PaintInfo& info) {
    SkCanvas* canvas = info.context ? info.context->canvas() : nullptr;
    if (!canvas || !m_layout || !m_layout->domElement() || !m_layout->domElement()->isTextField()) {
        return;
    }
    auto* field = static_cast<dom::TextFieldElement*>(m_layout->domElement());
    const css::Style& st = m_layout->style();

    SkRect box = info.paintRect;
    SkPaint bg;
    bg.setColor(skColorFromStyleColor(st.bgColor));
    bg.setAntiAlias(true);
    canvas->drawRect(box, bg);

    SkPaint border;
    border.setStyle(SkPaint::kStroke_Style);
    SkColor borderSk = skColorFromStyleColor(st.borderColor);
    if (borderSk == 0) {
        borderSk = SK_ColorGRAY;
    }
    border.setColor(borderSk);
    border.setStrokeWidth(std::max(1.f, st.borderTopWidth));
    border.setAntiAlias(true);
    canvas->drawRect(box, border);

    String drawText = field->value();
    if (drawText.empty() && !field->placeholder().empty()) {
        drawText = field->placeholder();
    }
    if (drawText.empty()) {
        return;
    }

    SkPaint textPaint;
    textPaint.setColor(skColorFromStyleColor(st.color));
    textPaint.setAntiAlias(true);
    float fontPx =
        st.fontSizePx > 0 ? static_cast<float>(st.fontSizePx) : 14.f;
    textPaint.setTextSize(fontPx);

    SkScalar baseline = box.y() + fontPx * 0.85f;
    canvas->drawText(drawText.c_str(), drawText.size(), box.x() + 6.f, baseline,
                     textPaint);
}

}  // namespace painter
}  // namespace boymue
