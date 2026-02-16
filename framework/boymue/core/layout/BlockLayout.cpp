#include "BlockLayout.h"
#include "ViewElement.h"

namespace boymue {
namespace layout {
BlockLayout::BlockLayout(dom::DocumentElement* element)
 : Layout(element) {}

void BlockLayout::paint(PaintInfo& info) {
    m_painter->paint(info);
}
}
}