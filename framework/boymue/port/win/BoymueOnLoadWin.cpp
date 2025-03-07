// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05

#include "BoymueOnLoadWin.h"

#include <stdio.h>

#include <string>
#include <fstream>

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
#include "BoymueBridge.h"
#include "FileUtil.h"
#include "StringUtil.h"
#include <jemalloc/jemalloc.h>

//#define USE_JEMALLOC

void* operator new(std::size_t sz) {
#ifdef USE_JEMALLOC
    return je_malloc(sz);
#else
    return malloc(sz);
#endif

}

void operator delete(void* p) {
#ifdef USE_JEMALLOC
    return je_free(p);
#else
    return free(p);
#endif
}

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

//static std::string s_projectPath = getenv("BOYMUE_ROOT"); 
static UIRuntime* s_uiRuntime;
static boymue::JsEngine* s_engine;

void BoymueOnLoadWin::initWindow(HWND hwnd, int width, int height) {
  
  boymue::PaintContextWin* painter = new boymue::PaintContextWin();
  painter->initContext(hwnd, width, height);
  UIRuntime* uiRuntime = new UIRuntime(painter, width, height);
  boymue::BoymueAppInfo* info = new boymue::BoymueAppInfo();
  info->appName = "example";
  boymue::BoymueApplication* app = new boymue::BoymueApplication(info);
  s_uiRuntime = uiRuntime;
  app->getUITaskRunner().postTask([=] { s_uiRuntime->run(); });
  
  boymue::String path = std::move(boymue::BoymueBridge::getSourcePath("\\example\\test.js"));
  boymue::String source = std::move(boymue::FileUtil::readFile(path));

  char* str = (char*)je_malloc(32);
  const char* src = "hello world";
  memset(str, 0, 32);
  memcpy(str, src, strlen(src));
  int i = 1000;
  while (i--) { je_malloc(4096); }


  app->evaluateJs(source.c_str(), path);
  m_app = app;
  
  boymue::Vector<char> buffer = {0x64, 0x66};

  boymue::HashMap<boymue::String, boymue::Vector<char>*> map;
  map["hello"] = &buffer;

  printf("test %ld", (intptr_t)map["world"]);
}

void BoymueOnLoadWin::repaint() {
  boymue::BoymueApplication* app = static_cast<boymue::BoymueApplication*>(m_app);
  app->getUITaskRunner().postTask([=] { s_uiRuntime->repaint(); });
}
