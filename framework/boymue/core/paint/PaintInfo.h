// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef PaintInfo_h
#define PaintInfo_h

#include "PaintContext.h"
#include "SkRect.h"

namespace boymue {
class PaintInfo {
 public:
  PaintInfo(){};

  SkRect rect;
  PaintContext* context;
};
}  // namespace boymue

#endif
