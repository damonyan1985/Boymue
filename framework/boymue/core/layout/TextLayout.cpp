#include "TextLayout.h"

namespace boymue {
namespace layout {
TextLayout::TextLayout(dom::DocumentElement* element)
    : Layout(element) {}

Layout::LayoutType TextLayout::type() const {
    return kLayoutText;
}

void TextLayout::layout() {
    syncMetricsFromStyle();
    LayoutUnit innerW = parentLayout() ? parentLayout()->contentWidth() : LayoutUnit(400);
    if (m_width <= 0.f) {
        m_width = innerW;
    }
    int fs = m_style.fontSizePx > 0 ? m_style.fontSizePx : 14;
    if (m_height <= 0.f) {
        m_height = static_cast<LayoutUnit>(fs * 1.35f);
    }
}
}
}  // namespace boymue
