// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef TextLayout_h
#define TextLayout_h

#include "Layout.h"
#include "TextElement.h"

namespace boymue {
   
namespace layout {
class TextLayout : public Layout {
 public:
  TextLayout(dom::DocumentElement* element);
  LayoutType type() const override;
  void layout() override;
};
}
}  // namespace boymue
#endif  // !TextLayout_h
