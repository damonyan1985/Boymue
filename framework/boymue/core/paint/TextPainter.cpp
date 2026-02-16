// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.30

#include "TextPainter.h"
#include "Layout.h"

namespace boymue {
namespace painter {
TextPainter::TextPainter(layout::Layout* layout)
    : Painter(layout) {}

void TextPainter::paintImpl(SkCanvas* canvas, PaintInfo& info) {}
}
}  // namespace boymue