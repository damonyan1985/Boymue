// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "Layout.h"

namespace boymue {
namespace layout {
Layout::Layout(dom::DocumentElement* element)
    : m_element(element) {}

Layout::~Layout() = default;

bool Layout::needsRepaint(const SkRect& paintRect) const {
    return m_needsRepaint || paintRect != m_lastPaintedRect;
}

void Layout::didRepaint(const SkRect& paintRect) {
    m_needsRepaint = false;
    m_lastPaintedRect = paintRect;
}

void Layout::invalidatePainter() {
    m_needsRepaint = true;
}

void Layout::ensurePainter() {
    if (!m_painter) {
        m_painter.reset(painter::Painter::createPainter(this));
    }
}

void Layout::layout() {}

Layout::LayoutType Layout::type() const {
    return kLayoutBlock;
}

const css::Style& Layout::style() const {
    return m_style;
}

css::Style& Layout::mutableStyle() {
    return m_style;
}

void Layout::syncMetricsFromStyle() {
    if (m_style.width > 0.f) {
        m_width = m_style.width;
    }
    if (m_style.height > 0.f) {
        m_height = m_style.height;
    }
}

LayoutUnit Layout::horizontalEdges() const {
    return m_style.borderLeftWidth + m_style.borderRightWidth + m_style.paddingLeft +
           m_style.paddingRight;
}

LayoutUnit Layout::verticalEdges() const {
    return m_style.borderTopWidth + m_style.borderBottomWidth + m_style.paddingTop +
           m_style.paddingBottom;
}

LayoutUnit Layout::contentWidth() const {
    LayoutUnit w = m_width - horizontalEdges();
    return w > 0.f ? w : 0.f;
}

LayoutUnit Layout::left() const {
    return m_left;
}
LayoutUnit Layout::top() const {
    return m_top;
}
LayoutUnit Layout::width() const {
    return m_width;
}
LayoutUnit Layout::height() const {
    return m_height;
}

void Layout::paint(PaintInfo& info) {
    if (m_style.display == css::DisplayValue::None) {
        return;
    }
    if (m_style.visibility == css::VisibilityValue::Hidden) {
        return;
    }
    if (!info.context || !info.context->canvas()) {
        return;
    }
    ensurePainter();
    if (m_painter) {
        m_painter->paint(info);
    }
}

dom::DocumentElement* Layout::element() const {
    return m_element;
}
}
}  // namespace boymue
