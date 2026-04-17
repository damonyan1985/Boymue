#include "ImageLayout.h"

#include "Document.h"
#include "ImageElement.h"

namespace boymue {
namespace layout {

ImageLayout::ImageLayout(dom::DocumentElement* element)
    : Layout(element)
    , m_image(std::make_unique<Image>()) {}

Image* ImageLayout::image() {
    return m_image.get();
}

Layout::LayoutType ImageLayout::type() const {
    return kLayoutImage;
}

void ImageLayout::onImageLoadComplete() {
    dom::DocumentElement* el = domElement();
    if (el && el->document()) {
        el->document()->requestRepaint(this);
    }
}

void ImageLayout::layout() {
    const bool specW = m_style.width > 0.f;
    const bool specH = m_style.height > 0.f;
    syncMetricsFromStyle();

    if (m_element && m_element->isImage()) {
        auto* imgEl = static_cast<dom::ImageElement*>(m_element);
        const String& src = imgEl->src();
        if (m_image && !src.empty() && src != m_issuedSrc) {
            m_issuedSrc = src;
            m_image->load(src, this);
        }
    }

    if (m_image && !m_image->bitmap().empty()) {
        if (!specW) {
            m_width = static_cast<LayoutUnit>(m_image->bitmap().width());
        }
        if (!specH) {
            m_height = static_cast<LayoutUnit>(m_image->bitmap().height());
        }
    } else {
        if (!specW) {
            m_width = parentLayout() ? parentLayout()->contentWidth() : LayoutUnit(100);
        }
        if (!specH) {
            m_height = LayoutUnit(24);
        }
    }
}
}
}  // namespace boymue
