// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#ifndef PaintContext_h
#define PaintContext_h

#include "SkCanvas.h"

namespace boymue {
class PaintContext {
 public:
  virtual void reset() = 0;
  virtual SkCanvas* canvas() = 0;
  virtual void submit() = 0;
};
}  // namespace boymue

#endif
