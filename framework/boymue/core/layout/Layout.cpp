// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "Layout.h"

namespace boymue {
namespace layout {
Layout::Layout(dom::DocumentElement* element) 
    : m_painter(painter::Painter::createPainter(this))
    , m_element(element) {}

Layout::~Layout() {}

void Layout::layout() {}

Layout::LayoutType Layout::type() const {
    return kLayoutBlock;
}

const css::Style& Layout::style() const {
    return m_style;
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
    if (m_painter) {
        m_painter->paint(info);
    }
}

dom::DocumentElement* Layout::element() const {
    return nullptr;
}
}
}  // namespace boymue