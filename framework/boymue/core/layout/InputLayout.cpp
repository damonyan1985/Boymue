#include "InputLayout.h"

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

}  // namespace layout
}  // namespace boymue
