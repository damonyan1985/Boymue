// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef Layout_h
#define Layout_h

#include "PaintInfo.h"

namespace boymue {
class Layout {
 public:
  virtual void layout();
  virtual void paint(PaintInfo& info) {};
};
}  // namespace boymue
#endif  // !Layout_h