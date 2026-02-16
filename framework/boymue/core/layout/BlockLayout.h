#ifndef BlockLayout_h
#define BlockLayout_h

#include "Layout.h"

namespace boymue {
namespace layout {
class BlockLayout : public Layout {
 public:
  BlockLayout(dom::DocumentElement* element);
  void paint(PaintInfo& info) override;
};
}
}  // namespace boymue
#endif  // !BlockLayout_h
