#ifndef BlockLayout_h
#define BlockLayout_h

#include "Layout.h"

namespace boymue {
namespace layout {
/// 块级格式化上下文：纵向堆叠子节点（对标 RenderBlockFlow 的极简版）
class BlockLayout : public Layout {
 public:
  explicit BlockLayout(dom::DocumentElement* element);
  LayoutType type() const override;
  void layout() override;
  void paint(PaintInfo& info) override;
};
}
}  // namespace boymue
#endif
