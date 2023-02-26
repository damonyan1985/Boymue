// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef TextPainter_h
#define TextPainter_h

#include "Painter.h"

namespace boymue {
namespace painter {
class TextPainter : public Painter {
 protected:
  virtual void paintImpl(SkCanvas* canvas, PaintInfo& info) override;
};
}
}  // namespace boymue
#endif  // !TextPainter_h
