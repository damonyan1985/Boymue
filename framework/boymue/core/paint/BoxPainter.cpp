#include "BoxPainter.h"

#include <algorithm>

#include "Layout.h"
#include "DocumentElement.h"
#include "Image.h"
#include "ImageLayout.h"
#include "TextElement.h"
#include "InputElement.h"
#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkRegion.h"
#include "SkRRect.h"
#include "SkRect.h"
#include "Style.h"

#include <cmath>

namespace boymue {
namespace painter {

static SkColor skColorFromStyleColor(const css::Color& c) {
    return static_cast<SkColor>(c.value());
}

namespace {

void buttonEffectivePadding(const css::Style& st, float& padL, float& padR, float& padT,
                            float& padB) {
    constexpr float kDefaultPadH = 10.f;
    constexpr float kDefaultPadV = 6.f;
    padL = st.paddingLeft > 0.f ? st.paddingLeft : kDefaultPadH;
    padR = st.paddingRight > 0.f ? st.paddingRight : kDefaultPadH;
    padT = st.paddingTop > 0.f ? st.paddingTop : kDefaultPadV;
    padB = st.paddingBottom > 0.f ? st.paddingBottom : kDefaultPadV;
}

void appendButtonSubtreeText(dom::DocumentElement* node, String& out) {
    if (!node) {
        return;
    }
    node->visitChildren([&](dom::DocumentElement* c) {
        if (!c) {
            return;
        }
        if (c->isText()) {
            out += static_cast<dom::TextElement*>(c)->text();
        } else {
            appendButtonSubtreeText(c, out);
        }
    });
}

String buttonLabelFromDom(dom::DocumentElement* root) {
    String s;
    appendButtonSubtreeText(root, s);
    return s;
}

}  // namespace

static SkVector cornerRadiiForRect(const css::BorderRadiusSize& sz, const SkRect& dst) {
    float rx = sz.horizontal;
    float ry = sz.vertical > 0.f ? sz.vertical : rx;
    if (!std::isfinite(rx)) {
        rx = 0.f;
    }
    if (!std::isfinite(ry)) {
        ry = 0.f;
    }
    const float mx = dst.width() * 0.5f;
    const float my = dst.height() * 0.5f;
    if (std::isfinite(mx) && rx > mx) {
        rx = mx;
    }
    if (std::isfinite(my) && ry > my) {
        ry = my;
    }
    SkVector v;
    v.set(rx, ry);
    return v;
}

static bool styleBuildRRect(const css::Style& st, const SkRect& box, SkRRect& out) {
    // NaN/Inf 的 paintRect 会让 width()/height() 与 0 的比较失效；SkRRect::setRectRadii 会
    // 清空 RRect，但旧代码仍把 rounded 当作 true，后续 clipRRect/drawRRect 可能崩溃。
    if (!st.hasBorderRadius() || !box.isFinite() || box.isEmpty()) {
        return false;
    }
    const SkVector radii[4] = {
        cornerRadiiForRect(st.borderTopLeftRadius, box),
        cornerRadiiForRect(st.borderTopRightRadius, box),
        cornerRadiiForRect(st.borderBottomRightRadius, box),
        cornerRadiiForRect(st.borderBottomLeftRadius, box),
    };
    out.setRectRadii(box, radii);
    return !out.isEmpty();
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
    const bool rounded = styleBuildRRect(st, box, rr);

    SkPaint fill;
    fill.setStyle(SkPaint::kFill_Style);
    fill.setColor(skColorFromStyleColor(st.bgColor));
    fill.setAntiAlias(true);
    if (rounded) {
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
        if (rounded) {
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
    const css::Style& st = m_layout->style();
    const SkRect dst = info.paintRect;

    SkRRect clipRr;
    const bool rounded = styleBuildRRect(st, dst, clipRr);

    auto* imgLay = static_cast<layout::ImageLayout*>(m_layout);
    Image* img = imgLay->image();
    if (!img) {
        return;
    }
    img->withBitmap([&](const SkBitmap& bmp) {
        if (bmp.empty()) {
            SkPaint p;
            p.setColor(SK_ColorLTGRAY);
            p.setAntiAlias(true);
            if (rounded) {
                canvas->drawRRect(clipRr, p);
            } else {
                canvas->drawRect(dst, p);
            }
            return;
        }
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setFilterQuality(kLow_SkFilterQuality);
        if (rounded) {
            canvas->save();
            canvas->clipRRect(clipRr, SkRegion::kIntersect_Op, true);
        }
        canvas->drawBitmapRect(bmp, dst, &paint);
        if (rounded) {
            canvas->restore();
        }
    });
}

InputPainter::InputPainter(layout::Layout* layout)
    : Painter(layout) {}

void InputPainter::paintImpl(PaintInfo& info) {
    SkCanvas* canvas = info.context ? info.context->canvas() : nullptr;
    if (!canvas || !m_layout || !m_layout->domElement() || !m_layout->domElement()->isTextField()) {
        return;
    }
    auto* field = static_cast<dom::InputElement*>(m_layout->domElement());
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

ButtonPainter::ButtonPainter(layout::Layout* layout)
    : Painter(layout) {}

void ButtonPainter::paintImpl(PaintInfo& info) {
    SkCanvas* canvas = info.context ? info.context->canvas() : nullptr;
    if (!canvas || !m_layout || !m_layout->domElement() ||
        !m_layout->domElement()->isButton()) {
        return;
    }
    const css::Style& st = m_layout->style();
    SkRect box = info.paintRect;

    SkRRect rr;
    const bool rounded = styleBuildRRect(st, box, rr);

    SkPaint fill;
    fill.setStyle(SkPaint::kFill_Style);
    fill.setColor(skColorFromStyleColor(st.bgColor));
    fill.setAntiAlias(true);
    if (rounded) {
        canvas->drawRRect(rr, fill);
    } else {
        canvas->drawRect(box, fill);
    }

    if (st.borderTopWidth > 0.f || st.borderRightWidth > 0.f || st.borderBottomWidth > 0.f ||
        st.borderLeftWidth > 0.f) {
        SkPaint border;
        border.setStyle(SkPaint::kStroke_Style);
        border.setColor(skColorFromStyleColor(st.borderColor));
        border.setStrokeWidth(std::max({st.borderTopWidth, st.borderRightWidth,
                                        st.borderBottomWidth, st.borderLeftWidth}));
        border.setAntiAlias(true);
        if (rounded) {
            canvas->drawRRect(rr, border);
        } else {
            canvas->drawRect(box, border);
        }
    }

    float padL = 0.f;
    float padR = 0.f;
    float padT = 0.f;
    float padB = 0.f;
    buttonEffectivePadding(st, padL, padR, padT, padB);

    const SkScalar bl = st.borderLeftWidth;
    const SkScalar br = st.borderRightWidth;
    const SkScalar bt = st.borderTopWidth;
    const SkScalar bb = st.borderBottomWidth;
    SkRect content = SkRect::MakeLTRB(
        box.left() + bl + padL, box.top() + bt + padT, box.right() - br - padR,
        box.bottom() - bb - padB);
    if (content.width() <= 0 || content.height() <= 0) {
        return;
    }

    const String label = buttonLabelFromDom(m_layout->domElement());
    if (label.empty()) {
        return;
    }

    const int fs = st.fontSizePx > 0 ? st.fontSizePx : 14;
    SkPaint textPaint;
    textPaint.setAntiAlias(true);
    textPaint.setTextSize(static_cast<float>(fs));
    textPaint.setColor(skColorFromStyleColor(st.color));

    const SkScalar tw = textPaint.measureText(label.c_str(), label.size());
    const SkScalar x = content.centerX() - tw * 0.5f;
    const SkScalar baseline = content.centerY() + static_cast<float>(fs) * 0.35f;
    canvas->drawText(label.c_str(), label.size(), x, baseline, textPaint);
}

}  // namespace painter
}  // namespace boymue
