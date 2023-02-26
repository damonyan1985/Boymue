// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef Painter_h
#define Painter_h

#include "PaintInfo.h"
//#include "Layout.h"

namespace boymue {
namespace layout {
class Layout;
}
namespace painter {
class Painter {
 public:
  static Painter* createPainter(layout::Layout* layout);
  Painter(layout::Layout* layout);

  virtual ~Painter();
  virtual void paint(PaintInfo& info);
  SkPicture* picture() const;

 protected:
  virtual void paintImpl(SkCanvas* canvas, PaintInfo& info) = 0;
  // 存储渲染节点绘制指令
  SkPicture* m_picture;
  layout::Layout* m_layout;
};
}
}  // namespace boymue
#endif  // !Painter_h
