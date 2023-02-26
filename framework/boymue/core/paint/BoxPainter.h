// Box model paint
#ifndef BoxPainter_h
#define BoxPainter_h

#include "Painter.h"

namespace boymue {
namespace painter {
class BoxPainter : public Painter {
public:
    BoxPainter(layout::Layout* layout);
protected:
    virtual void paintImpl(SkCanvas* canvas, PaintInfo& info) override;
};
}
}
#endif