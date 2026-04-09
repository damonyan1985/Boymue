#ifndef Image_h
#define Image_h

#include "SkBitmap.h"

namespace boymue {
class Image {
public:
    Image();
    void createImage(const void* buffer, size_t size);
    /// 从本地路径解码（对标资源/文件加载）
    bool loadFromFile(const char* path);
    const SkBitmap& bitmap() const;

private:
    SkBitmap m_bitmap;
};
}

#endif // !Image_h
