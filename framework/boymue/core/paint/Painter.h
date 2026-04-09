// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef Painter_h
#define Painter_h

#include "PaintInfo.h"

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
  /// 绘制到 info.context->canvas()（内部调用 paintImpl）
  void paint(PaintInfo& info);

 protected:
  virtual void paintImpl(PaintInfo& info) = 0;
  SkPicture* m_picture;
  layout::Layout* m_layout;
};
}
}  // namespace boymue
#endif  // !Painter_h
