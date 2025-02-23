#include "ImageLayout.h"
#include "SkImageDecoder.h"

namespace boymue {
namespace layout {
ImageLayout::ImageLayout() : m_image(std::make_unique<Image>()) {}

Image* ImageLayout::image() { return m_image.get(); }

void ImageLayout::layout() {

}
}
}