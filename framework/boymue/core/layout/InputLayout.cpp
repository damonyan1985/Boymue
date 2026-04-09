#include "InputLayout.h"

#include "DocumentElement.h"
#include "TextElement.h"
#include "SkPaint.h"

namespace {

void buttonEffectivePadding(const boymue::css::Style& st, float& padL, float& padR, float& padT,
                            float& padB) {
    constexpr float kDefaultPadH = 10.f;
    constexpr float kDefaultPadV = 6.f;
    padL = st.paddingLeft > 0.f ? st.paddingLeft : kDefaultPadH;
    padR = st.paddingRight > 0.f ? st.paddingRight : kDefaultPadH;
    padT = st.paddingTop > 0.f ? st.paddingTop : kDefaultPadV;
    padB = st.paddingBottom > 0.f ? st.paddingBottom : kDefaultPadV;
}

void appendButtonSubtreeText(boymue::dom::DocumentElement* node, boymue::String& out) {
    if (!node) {
        return;
    }
    node->visitChildren([&](boymue::dom::DocumentElement* c) {
        if (!c) {
            return;
        }
        if (c->isText()) {
            out += static_cast<boymue::dom::TextElement*>(c)->text();
        } else {
            appendButtonSubtreeText(c, out);
        }
    });
}

boymue::String buttonLabelFromDom(boymue::dom::DocumentElement* root) {
    boymue::String s;
    appendButtonSubtreeText(root, s);
    return s;
}

}  // namespace

namespace boymue {
namespace layout {

InputLayout::InputLayout(dom::DocumentElement* element)
    : Layout(element) {}

Layout::LayoutType InputLayout::type() const {
    return kLayoutInput;
}

void InputLayout::layout() {
    syncMetricsFromStyle();
    LayoutUnit innerW = parentLayout() ? parentLayout()->contentWidth() : LayoutUnit(400);
    if (m_width <= 0.f) {
        m_width = innerW;
    }
    int fs = m_style.fontSizePx > 0 ? m_style.fontSizePx : 14;
    if (m_height <= 0.f) {
        m_height = static_cast<LayoutUnit>(fs * 2.2f);
    }
}

ButtonLayout::ButtonLayout(dom::DocumentElement* element)
    : Layout(element) {}

Layout::LayoutType ButtonLayout::type() const {
    return kLayoutButton;
}

void ButtonLayout::layout() {
    syncMetricsFromStyle();

    const String label = buttonLabelFromDom(domElement());
    const int fs = m_style.fontSizePx > 0 ? m_style.fontSizePx : 14;

    float padL = 0.f;
    float padR = 0.f;
    float padT = 0.f;
    float padB = 0.f;
    buttonEffectivePadding(m_style, padL, padR, padT, padB);

    const LayoutUnit borderLR =
        m_style.borderLeftWidth + m_style.borderRightWidth;
    const LayoutUnit borderTB =
        m_style.borderTopWidth + m_style.borderBottomWidth;

    SkPaint measurePaint;
    measurePaint.setTextSize(static_cast<float>(fs));
    LayoutUnit textW = 0.f;
    if (!label.empty()) {
        textW = static_cast<LayoutUnit>(
            measurePaint.measureText(label.c_str(), label.size()));
    }

    constexpr LayoutUnit kEmptyMinContentW = 40.f;
    const LayoutUnit contentW =
        (label.empty() ? kEmptyMinContentW : textW) + padL + padR;
    const LayoutUnit lineH = static_cast<LayoutUnit>(fs * 1.2f);
    const LayoutUnit contentH = lineH + padT + padB;

    if (m_width <= 0.f) {
        m_width = contentW + borderLR;
    }
    if (m_height <= 0.f) {
        m_height = contentH + borderTB;
    }
}

}  // namespace layout
}  // namespace boymue
