#include "Image.h"
#include "SkImageDecoder.h"

namespace boymue {
Image::Image() {}

void Image::createImage(const void* buffer, size_t size) {
    SkImageDecoder::DecodeMemory(buffer, size, &m_bitmap);
}

const SkBitmap& Image::bitmap() const { return m_bitmap; }
}