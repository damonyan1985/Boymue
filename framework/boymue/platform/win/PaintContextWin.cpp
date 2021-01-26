// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#include "PaintContextWin.h"

#include "SkSurface.h"

namespace boymue {
PaintContextWin::~PaintContextWin() {
  if (m_surface) {
    m_surface->unref();
  }
}

void PaintContextWin::initContext(HWND hwnd, int width, int height) {
  m_hwnd = hwnd;
  HDC hdc = GetDC(m_hwnd);
  RECT rt;
  GetClientRect(m_hwnd, &rt);
  int bmpw = rt.right - rt.left;
  int bmph = rt.bottom - rt.top;

  const size_t bmpSize =
      sizeof(BITMAPINFOHEADER) + bmpw * bmph * sizeof(uint32_t);
  m_bitmapInfo = (BITMAPINFO*)new BYTE[bmpSize]();
  m_bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  m_bitmapInfo->bmiHeader.biWidth = bmpw;

  m_bitmapInfo->bmiHeader.biHeight = -bmph;

  m_bitmapInfo->bmiHeader.biPlanes = 1;
  m_bitmapInfo->bmiHeader.biBitCount = 32;
  m_bitmapInfo->bmiHeader.biCompression = BI_RGB;
  void* pixels = m_bitmapInfo->bmiColors;

  SkImageInfo info = SkImageInfo::Make(bmpw, bmph, kBGRA_8888_SkColorType,
                                       kPremul_SkAlphaType);

  m_surface = SkSurface::NewRasterDirect(info, pixels, bmpw * sizeof(uint32_t));
}
void PaintContextWin::reset() {}

SkCanvas* PaintContextWin::canvas() { return m_surface->getCanvas(); }

void PaintContextWin::submit() {
  HDC hdc = GetDC(m_hwnd);
  RECT rt;
  GetClientRect(m_hwnd, &rt);
  int bmpw = rt.right - rt.left;
  int bmph = rt.bottom - rt.top;

  StretchDIBits(hdc, 0, 0, bmpw, bmph, 0, 0, bmpw, bmph,
                m_bitmapInfo->bmiColors, m_bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}
}  // namespace boymue