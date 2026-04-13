// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#ifndef Layout_h
#define Layout_h

#include "PaintInfo.h"
#include "Painter.h"
#include "Style.h"
#include "DocumentElement.h"

namespace boymue {
namespace layout {
// Create and run in ui thread
class Layout {
public:
    enum LayoutType {
        kLayoutBlock,
        kLayoutImage,
        kLayoutInput,
        kLayoutButton,
        kLayoutText
    };
    Layout(dom::DocumentElement* element);
    virtual ~Layout();

    virtual LayoutType type() const;
    virtual void layout();
    virtual void paint(PaintInfo& info);

    const css::Style& style() const;
    css::Style& mutableStyle();
    void syncMetricsFromStyle();

    LayoutUnit left() const;
    LayoutUnit top() const;
    LayoutUnit width() const;
    LayoutUnit height() const;

    dom::DocumentElement* domElement() const { return m_element; }

    /// 当前 paintRect 下是否需要重新录制 SkPicture（样式/DOM 失效或绘制区域变化）。
    bool needsRepaint(const SkRect& paintRect) const;
    /// Painter 完成一次录制后调用，清除重绘需求并记录本次 paintRect。
    void didRepaint(const SkRect& paintRect);
    void invalidatePainter();

protected:
    virtual dom::DocumentElement* element() const;
    void setParentLayout(Layout* p) { m_parentLayout = p; }
    Layout* parentLayout() const { return m_parentLayout; }
    LayoutUnit horizontalEdges() const;
    LayoutUnit verticalEdges() const;
    LayoutUnit contentWidth() const;
    void ensurePainter();

    Layout* m_parentLayout{nullptr};
    LayoutUnit m_left;
    LayoutUnit m_top;
    LayoutUnit m_width;
    LayoutUnit m_height;

    css::Style m_style;
    
    OwnerPtr<painter::Painter> m_painter;
    dom::DocumentElement* m_element;
    bool m_needsRepaint{true};
    SkRect m_lastPaintedRect{SkRect::MakeEmpty()};

    friend class BlockLayout;
    friend class ImageLayout;
    friend class TextLayout;
    friend class InputLayout;
    friend class ButtonLayout;
};
}
}  // namespace boymue
#endif  // !Layout_h