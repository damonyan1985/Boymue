#include "ImageLayout.h"

#include "Document.h"
#include "ImageElement.h"
#include "StringUtil.h"

namespace boymue {
namespace layout {

namespace {

void onImageResourceLoaded(bool success, const SkBitmap&, void* userData) {
    (void)success;
    auto* self = static_cast<ImageLayout*>(userData);
    dom::DocumentElement* el = self ? self->domElement() : nullptr;
    if (el && el->document()) {
        el->document()->requestRepaint(self);
    }
}

}  // namespace

ImageLayout::ImageLayout(dom::DocumentElement* element)
    : Layout(element)
    , m_image(std::make_unique<Image>()) {}

Image* ImageLayout::image() {
    return m_image.get();
}

Layout::LayoutType ImageLayout::type() const {
    return kLayoutImage;
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
            const bool network = StringUtil::startWith(src, "http://") ||
                                 StringUtil::startWith(src, "https://");
            if (network) {
                m_image->load(src.c_str(), onImageResourceLoaded, this);
            } else {
                m_image->loadFromFile(src.c_str());
            }
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
