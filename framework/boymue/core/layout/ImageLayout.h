#ifndef ImageLayout_h
#define ImageLayout_h

#include "Layout.h"
#include "Image.h"

namespace boymue {
namespace layout {

class ImageLayout : public Layout {
public:
    ImageLayout(dom::DocumentElement* element);
    virtual LayoutType type() const override;
    virtual void layout();
    virtual void paint(PaintInfo& info) override;

    Image* image();

private:
    OwnerPtr<Image> m_image;
};
}
}  // namespace boymue
#endif  // !ImageLayout_h
