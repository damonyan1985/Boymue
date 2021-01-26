// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#ifndef PaintContextWin_h
#define PaintContextWin_h

#include <windows.h>

#include "PaintContext.h"
#include "SkBitmap.h"
#include "SkSurface.h"

namespace boymue {
class PaintContextWin : public PaintContext {
 public:
  ~PaintContextWin();
  void initContext(HWND hwnd, int width, int height);
  virtual void reset();
  virtual SkCanvas* canvas();
  virtual void submit();

 private:
  HWND m_hwnd;
  SkBitmap* m_skbitmap;
  BITMAPINFO* m_bitmapInfo;
  SkSurface* m_surface;
};
}  // namespace boymue
#endif