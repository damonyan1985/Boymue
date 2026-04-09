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
#include "Document.h"
#include "FileUtil.h"
#include "Layout.h"
#include "PaintContext.h"
#include "PaintInfo.h"
#include "SkRect.h"
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

namespace {

/// 示例：内含 Boymue DOM 标签的 XML，经 Document 解析（含 &lt;style&gt; 内 CSS）、
/// StyleEngine::apply、布局与绘制，输出到给定 SkCanvas（再由 UIRuntime 录制成图并 submit 到窗口）。
void RenderDomXmlToWindow(SkCanvas* canvas, int width, int height) {
  if (!canvas || width <= 0 || height <= 0) {
    return;
  }

  boymue::dom::Document document;
  document.frame().setViewport(static_cast<boymue::LayoutUnit>(width),
                               static_cast<boymue::LayoutUnit>(height));

  std::string xml;
  xml.reserve(640);
  xml += "<view><style>\n";
  xml += "view { width: ";
  xml += std::to_string(width);
  xml += "px; height: ";
  xml += std::to_string(height);
  xml += "px; background-color: rgb(60,60,250); }\n";
  xml += "button { width: 220px; height: 44px; margin-top: 24px; margin-left: 24px; color: rgb(60,60,250); ";
  xml += "background-color: rgb(200,60,60); border-radius: 8px; }\n";
  xml += "</style><button>Test</button></view>";

  document.parseFromXML(xml);

  boymue::dom::DocumentElement* root = document.root();
  if (!root) {
    return;
  }

  boymue::layout::Layout* rootLayout = root->createLayout();
  if (!rootLayout) {
    return;
  }
  rootLayout->layout();

  boymue::RecordingPaintContext paintCtx(canvas);
  boymue::PaintInfo info;
  info.context = &paintCtx;
  info.paintRect = SkRect::MakeIWH(width, height);
  info.clipRect = info.paintRect;
  rootLayout->paint(info);
}

}  // namespace

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

  void Draw(SkCanvas* canvas) {
    RenderDomXmlToWindow(canvas, m_width, m_height);
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
