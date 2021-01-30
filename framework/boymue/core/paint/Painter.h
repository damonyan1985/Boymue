// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef Painter_h
#define Painter_h

#include "PaintInfo.h"

namespace boymue {
class Painter {
 public:
  Painter();
  virtual ~Painter();
  virtual void paint(PaintInfo& context);

 protected:
  virtual void paintImpl(SkCanvas* canvas) = 0;

 private:
  // 存储渲染节点绘制指令
  SkPicture* m_picture;
};
}  // namespace boymue
#endif  // !Painter_h
