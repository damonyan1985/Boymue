// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef Layout_h
#define Layout_h

#include "PaintInfo.h"
#include "Style.h"

namespace boymue {
using LayoutUnit = int;
// Create and run in ui thread
class Layout {
public:
    virtual void layout();
    virtual void paint(PaintInfo& info){};

private:
    LayoutUnit m_left;
    LayoutUnit m_top;

    css::Style m_style;
};
}  // namespace boymue
#endif  // !Layout_h