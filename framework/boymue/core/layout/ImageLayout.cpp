#include "ImageLayout.h"
#include "SkImageDecoder.h"
#include "ImageElement.h"

namespace boymue {
namespace layout {
ImageLayout::ImageLayout(dom::DocumentElement* element) 
    : Layout(element)
    , m_image(std::make_unique<Image>()) {}

Image* ImageLayout::image() { return m_image.get(); }

Layout::LayoutType ImageLayout::type() const {
    return kLayoutImage;
}

void ImageLayout::layout() {

}

void ImageLayout::paint(PaintInfo& info) {
    if (m_painter) {
        m_painter->paint(info);
    }
}
}
}