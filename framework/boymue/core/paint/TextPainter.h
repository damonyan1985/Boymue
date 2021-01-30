// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef TextPainter_h
#define TextPainter_h

#include "Painter.h"

namespace boymue {
class TextPainter : public Painter {
 public:
  virtual void paint(PaintInfo& context);

 protected:
  virtual void paintImpl(SkCanvas* canvas) override;
};
}  // namespace boymue
#endif  // !TextPainter_h
