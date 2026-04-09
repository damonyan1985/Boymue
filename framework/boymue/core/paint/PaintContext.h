// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#ifndef PaintContext_h
#define PaintContext_h

#include "SkCanvas.h"

namespace boymue {
// 图形绘制上下文
class PaintContext {
 public:
  virtual void reset() = 0;
  virtual SkCanvas* canvas() = 0;
  virtual void submit() = 0;
  virtual ~PaintContext() = default;
};

/// 将 SkPictureRecorder 等产生的 SkCanvas 挂到 PaintInfo::context 上
class RecordingPaintContext final : public PaintContext {
 public:
  explicit RecordingPaintContext(SkCanvas* canvas) : m_canvas(canvas) {}
  void reset() override {}
  SkCanvas* canvas() override { return m_canvas; }
  void submit() override {}

 private:
  SkCanvas* m_canvas;
};
}  // namespace boymue

#endif
