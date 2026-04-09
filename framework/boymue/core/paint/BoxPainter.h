// Box model paint — block 容器、图片与表单控件绘制
#ifndef BoxPainter_h
#define BoxPainter_h

#include "Painter.h"

namespace boymue {
namespace painter {

class BoxPainter : public Painter {
public:
    explicit BoxPainter(layout::Layout* layout);

protected:
    void paintImpl(PaintInfo& info) override;
};

class ImagePainter : public Painter {
public:
    explicit ImagePainter(layout::Layout* layout);

protected:
    void paintImpl(PaintInfo& info) override;
};

class InputPainter : public Painter {
public:
    explicit InputPainter(layout::Layout* layout);

protected:
    void paintImpl(PaintInfo& info) override;
};

}  // namespace painter
}  // namespace boymue
#endif
