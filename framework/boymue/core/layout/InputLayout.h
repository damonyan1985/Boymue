#ifndef InputLayout_h
#define InputLayout_h

#include "Layout.h"

namespace boymue {
namespace layout {
class InputLayout : public Layout {
 public:
  explicit InputLayout(dom::DocumentElement* element);
  LayoutType type() const override;
  void layout() override;
};

/// 对标 WebKit RenderButton：内容区默认内边距 + 标签居中（绘制见 BoxPainter::ButtonPainter）。
class ButtonLayout : public Layout {
 public:
  explicit ButtonLayout(dom::DocumentElement* element);
  LayoutType type() const override;
  void layout() override;
};
}  // namespace layout
}  // namespace boymue
#endif
