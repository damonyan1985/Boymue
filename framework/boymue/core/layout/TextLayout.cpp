#include "TextLayout.h"


namespace boymue {
namespace layout {
TextLayout::TextLayout(dom::DocumentElement* element)
    : Layout(element) {}

Layout::LayoutType TextLayout::type() const {
  return kLayoutText;
}

void TextLayout::paint(PaintInfo& info) {
  if (m_painter) {
    m_painter->paint(info);
  }
}

dom::TextElement* TextLayout::element() const {
  return static_cast<dom::TextElement*>(m_element);
}
}
}  // namespace boymue

