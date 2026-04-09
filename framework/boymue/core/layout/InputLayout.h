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
}
}  // namespace boymue
#endif
