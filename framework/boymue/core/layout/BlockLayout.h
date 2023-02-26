#ifndef BlockLayout_h
#define BlockLayout_h

#include "Layout.h"

namespace boymue {
namespace layout {
class BlockLayout : public Layout {
public:
    virtual void paint(PaintInfo& info);
};
}
}  // namespace boymue
#endif  // !BlockLayout_h
