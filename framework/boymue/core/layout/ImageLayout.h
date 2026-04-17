#ifndef ImageLayout_h
#define ImageLayout_h

#include "Layout.h"
#include "Image.h"

namespace boymue {
namespace layout {

class ImageLayout : public Layout, public ImageLoadClient {
public:
    ImageLayout(dom::DocumentElement* element);
    virtual LayoutType type() const override;
    virtual void layout();

    Image* image();

    void onImageLoadComplete() override;

private:
    OwnerPtr<Image> m_image;
    /// 避免 layout 重复触发同一路径的加载
    String m_issuedSrc;
};
}
}  // namespace boymue
#endif  // !ImageLayout_h
