#include "BoymueOnLoadWin.h"

#include <stdio.h>

#include <string>

#include "BoymueView.h"
#include "PaintContextWin.h"
#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkString.h"
#include "SkSurface.h"
#include "TaskThread.h"
#include "Thread.h"

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
class UIRuntime {
 public:
  UIRuntime(boymue::PaintContextWin* painer) : m_painter(painer) {}

  virtual void run() {
    SkCanvas* canvas = m_painter->canvas();

    canvas->clear(SK_ColorWHITE);

    Draw(canvas);

    m_painter->submit();
  }

  void WcharToChar(const wchar_t* wp, std::string& text, size_t encode) {
    // std::string str;
    int len =
        WideCharToMultiByte(encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
    char* chs = new char[len + 1];
    WideCharToMultiByte(encode, 0, wp, wcslen(wp), chs, len, NULL, NULL);
    chs[len] = '\0';
    text = chs;
  }

  void Draw(SkCanvas* canvas) {
    SkPaint paint;
    paint.setStrokeWidth(1);
    paint.setARGB(0xff, 0xff, 0, 0);
    canvas->drawRect(SkRect::MakeXYWH(10, 10, 100, 100), paint);

    SkPaint textpaint;
    textpaint.setTextSize(16);
    textpaint.setColor(SK_ColorRED);
    textpaint.setAntiAlias(true);
    // SkString string("Hello World");
    const wchar_t* text = L"Hello World";
    std::string str;
    WcharToChar(text, str, CP_UTF8);
    canvas->drawText(str.c_str(), str.size(), 20, 300, textpaint);
    // canvas->restore();
    // canvas->flush();
  }

  void repaint() { m_painter->submit(); }

 private:
  boymue::PaintContextWin* m_painter;
};

static UIRuntime* s_uiRuntime;
static boymue::BoymueView* s_view;

void BoymueOnLoadWin::initWindow(HWND hwnd, int width, int height) {
  boymue::PaintContextWin* painter = new boymue::PaintContextWin();
  painter->initContext(hwnd, width, height);
  s_uiRuntime = new UIRuntime(painter);
  s_view = new boymue::BoymueView();

  s_view->getTaskRunner().postTask([=] { s_uiRuntime->run(); });
}

void BoymueOnLoadWin::repaint() {
  s_view->getTaskRunner().postTask([=] { s_uiRuntime->repaint(); });
}
