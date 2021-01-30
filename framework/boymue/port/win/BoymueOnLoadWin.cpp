#include "BoymueOnLoadWin.h"

#include <stdio.h>

#include <string>

#include "BoymueApplication.h"
#include "BoymueView.h"
#include "JsEngine.h"
#include "JsLogApi.h"
#include "PaintContextWin.h"
#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkPictureRecorder.h"
#include "SkString.h"
#include "SkSurface.h"
#include "TaskThread.h"
#include "Thread.h"

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
class UIRuntime {
 public:
  UIRuntime(boymue::PaintContextWin* painter, int width, int height)
      : m_painter(painter), m_width(width), m_height(height) {}

  virtual void run() {
    SkCanvas* canvas = m_painter->canvas();

    canvas->clear(SK_ColorWHITE);

    SkPictureRecorder recorder;
    SkCanvas* recorderCanvas =
        recorder.beginRecording(m_width, m_height, NULL, 0);
    Draw(recorderCanvas);
    SkPicture* picture = recorder.endRecording();
    canvas->drawPicture(picture);
    picture->unref();
    canvas->unref();

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
  int m_width;
  int m_height;
};

static UIRuntime* s_uiRuntime;
static boymue::BoymueApplication* s_app;
static boymue::JsEngine* s_engine;

void BoymueOnLoadWin::initWindow(HWND hwnd, int width, int height) {
  boymue::PaintContextWin* painter = new boymue::PaintContextWin();
  painter->initContext(hwnd, width, height);
  s_uiRuntime = new UIRuntime(painter, width, height);
  s_app = new boymue::BoymueApplication();

  s_app->getUITaskRunner().postTask([=] { s_uiRuntime->run(); });

  // s_engine = new boymue::JsEngine();
  // boymue::JsRuntime* runtime = s_engine->createRuntime();
  // runtime->registerApi(new boymue::JsLogApi());

  s_app->evaluateJs(
      "function test(a, b) { let arr = [0, 1, 2];arr.push(5);boymue.log('test "
      "arr length='+arr.length);return a * b * arr[2];} boymue.log(test(2, "
      "3));");
}

void BoymueOnLoadWin::repaint() {
  s_app->getUITaskRunner().postTask([=] { s_uiRuntime->repaint(); });
}
