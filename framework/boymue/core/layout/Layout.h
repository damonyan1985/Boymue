// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef Layout_h
#define Layout_h

#include "PaintInfo.h"
#include "Style.h"
#include "Painter.h"

namespace boymue {
using LayoutUnit = int;
namespace layout {
// Create and run in ui thread
class Layout {
public:
    enum LayoutType {
        kLayoutBlock,
        kLayoutImage,
        kLayoutInput,
        kLayoutText
    };

    virtual LayoutType type() const;
    virtual void layout();
    virtual void paint(PaintInfo& info){};

    const css::Style& style() const;
    LayoutUnit left() const;
    LayoutUnit top() const;
    LayoutUnit width() const;
    LayoutUnit height() const;

protected:
    LayoutUnit m_left;
    LayoutUnit m_top;
    LayoutUnit m_width;
    LayoutUnit m_height;

    css::Style m_style;
    OwnerPtr<painter::Painter> m_painter;
};
}
}  // namespace boymue
#endif  // !Layout_h