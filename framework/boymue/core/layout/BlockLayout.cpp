#include "BlockLayout.h"

#include "ViewElement.h"

namespace boymue {
namespace layout {

BlockLayout::BlockLayout(dom::DocumentElement* element)
    : Layout(element) {}

Layout::LayoutType BlockLayout::type() const {
    return kLayoutBlock;
}

void BlockLayout::layout() {
    syncMetricsFromStyle();

    LayoutUnit innerW = parentLayout() ? parentLayout()->contentWidth() : LayoutUnit(400);
    if (m_width <= 0.f) {
        m_width = innerW;
    }

    const css::Style& st = m_style;
    LayoutUnit x0 = m_left + st.borderLeftWidth + st.paddingLeft;
    LayoutUnit y0 = m_top + st.borderTopWidth + st.paddingTop;
    LayoutUnit innerContentW = contentWidth();
    LayoutUnit cy = y0;

    if (m_element && m_element->isView()) {
        auto* ve = static_cast<dom::ViewElement*>(m_element);
        ve->visitChildren([&](dom::DocumentElement* child) {
            if (!child) {
                return;
            }
            Layout* cl = child->createLayout();
            if (!cl) {
                return;
            }
            if (cl->style().display == css::DisplayValue::None) {
                return;
            }
            cl->setParentLayout(this);
            cl->layout();

            const css::Style& cs = cl->style();
            LayoutUnit ml = cs.marginLeft;
            LayoutUnit mr = cs.marginRight;
            LayoutUnit mt = cs.marginTop;
            LayoutUnit mb = cs.marginBottom;

            if ((child->isView() || child->isButton() || child->isIframe()) && cl->width() <= 0.f) {
                cl->m_width = innerContentW - ml - mr;
                if (cl->m_width < 0.f) {
                    cl->m_width = 0.f;
                }
                cl->layout();
            }

            cl->m_left = x0 + ml;
            cl->m_top = cy + mt;
            cy += mt + cl->height() + mb;
        });
    }

    if (m_height <= 0.f) {
        LayoutUnit contentH = cy - y0;
        if (contentH < 0.f) {
            contentH = 0.f;
        }
        m_height = contentH + verticalEdges();
    }
}

void BlockLayout::paint(PaintInfo& info) {
    if (m_style.display == css::DisplayValue::None) {
        return;
    }
    if (m_style.visibility == css::VisibilityValue::Hidden) {
        return;
    }
    Layout::paint(info);

    if (!m_element || !m_element->isView() || !info.context || !info.context->canvas()) {
        return;
    }
    auto* ve = static_cast<dom::ViewElement*>(m_element);
    ve->visitChildren([&](dom::DocumentElement* child) {
        Layout* cl = child ? child->layout() : nullptr;
        if (!cl || cl->style().display == css::DisplayValue::None) {
            return;
        }
        PaintInfo ci = info;
        ci.context = info.context;
        ci.paintRect =
            SkRect::MakeXYWH(cl->left(), cl->top(), cl->width(), cl->height());
        ci.clipRect = ci.paintRect;
        cl->paint(ci);
    });
}

}  // namespace layout
}  // namespace boymue
