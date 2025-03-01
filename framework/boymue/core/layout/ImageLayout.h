#ifndef ImageLayout_h
#define ImageLayout_h

#include "Layout.h"
#include "Image.h"

namespace boymue {
namespace layout {

class ImageLayout : public Layout {
public:
    ImageLayout();
    virtual void layout();

    Image* image();

private:
    OwnerPtr<Image> m_image;
};
}
}  // namespace boymue
#endif  // !ImageLayout_h
