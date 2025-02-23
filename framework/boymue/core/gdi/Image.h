#ifndef Image_h
#define Image_h

#include "SkBitmap.h"

namespace boymue {
class Image {
public:
	Image();
	void createImage(const void* buffer, size_t size);
	const SkBitmap& bitmap() const;

private:
	SkBitmap m_bitmap;
};
}

#endif // !Image_h
