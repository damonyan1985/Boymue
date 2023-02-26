#include "BoxPainter.h"
#include "Layout.h"

namespace boymue {
namespace painter {
BoxPainter::BoxPainter(layout::Layout* layout)
    : Painter(layout){}

void BoxPainter::paintImpl(SkCanvas* canvas, PaintInfo& info) {
    SkPaint paint;
    paint.setColor(m_layout->style().bgColor.value());
    canvas->drawRect(info.paintRect, paint);
}
}
}  // namespace boymue