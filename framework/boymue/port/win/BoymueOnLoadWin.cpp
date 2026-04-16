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
#include "DocumentElement.h"
#include <functional>
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

class UIRuntime;

static UIRuntime* s_uiRuntime = nullptr;
static boymue::BoymueApplication* s_domRepaintApp = nullptr;

static void PostDomXmlRepaint();

/// 示例 XML：view + style + button + 在线图片（picsum）
static boymue::String BuildDemoDomXml(int width, int height) {
  boymue::String xml;
  xml.reserve(1200);
  xml += "<view><style>\n";
  xml += "view { width: ";
  xml += std::to_string(width);
  xml += "px; height: ";
  xml += std::to_string(height);
  xml += "px; background-color: rgb(60,60,250); color: rgb(60,60,250); }\n";
  xml += "button { width: 220px; height: 44px; margin-top: 24px; margin-left: 24px; ";
  xml += "background-color: rgb(200,60,60); border-radius: 8px; }\n";
  xml += "img { width: 280px; height: 180px; margin-top: 20px; margin-left: 24px; ";
  xml += "border-radius: 8px; }\n";
  xml += "</style><button>Test</button>";
  xml += "<img src=\"https://fastly.picsum.photos/id/805/280/180.jpg?hmac=Mk6FV2pjw4wjaHLGtrtYET7H2qevT0N7-Zx0ayyQWdQ\" />";
  xml += "</view>";
  return xml;
}

/// 对已解析的 Document 做视口、布局与绘制（Document 须由宿主保持存活至异步图片完成）
static void RenderDomXmlToWindow(SkCanvas* canvas, int width, int height,
                                 boymue::dom::Document& document) {
  if (!canvas || width <= 0 || height <= 0) {
    return;
  }

  document.frame().setViewport(static_cast<boymue::LayoutUnit>(width),
                               static_cast<boymue::LayoutUnit>(height));

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
  // 各 Layout 的 Painter 先将内容录到 SkPicture，再在 Painter::paint 末尾回放到本 canvas
  rootLayout->paint(info);
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

    m_painter->submit();
  }

  void Draw(SkCanvas* canvas) {
    ensureDomParsed();
    if (m_document) {
      RenderDomXmlToWindow(canvas, m_width, m_height, *m_document);
    }
  }

  void repaint() { 
    //m_painter->submit();
    run(); 
  }

  /// 使整棵布局树的 Painter 缓存失效（异步资源如网络图到位后须在 UI 线程调用）
  void invalidateDomPainters();

 private:
  void ensureDomParsed();

  boymue::PaintContextWin* m_painter;
  int m_width;
  int m_height;
  boymue::OwnerPtr<boymue::dom::Document> m_document;
  bool m_domParsed = false;
};

static void PostDomXmlRepaint() {
  if (!s_domRepaintApp || !s_uiRuntime) {
    return;
  }
  s_domRepaintApp->getUITaskRunner().postTask([] {
    if (s_uiRuntime) {
      s_uiRuntime->invalidateDomPainters();
      s_uiRuntime->repaint();
    }
  });
}

void UIRuntime::invalidateDomPainters() {
  if (!m_document || !m_document->root()) {
    return;
  }
  std::function<void(boymue::dom::DocumentElement*)> visit;
  visit = [&](boymue::dom::DocumentElement* el) {
    if (!el) {
      return;
    }
    if (boymue::layout::Layout* lay = el->layout()) {
      lay->invalidatePainter();
    }
    el->visitChildren([&](boymue::dom::DocumentElement* c) { visit(c); });
  };
  visit(m_document->root());
}

void UIRuntime::ensureDomParsed() {
  if (m_domParsed) {
    return;
  }
  m_document.reset(new boymue::dom::Document());
  m_document->frame().setViewport(static_cast<boymue::LayoutUnit>(m_width),
                                   static_cast<boymue::LayoutUnit>(m_height));
  m_document->setRepaintCallback(PostDomXmlRepaint);
  m_document->parseFromXML(BuildDemoDomXml(m_width, m_height));
  m_domParsed = true;
}

//static std::string s_projectPath = getenv("BOYMUE_ROOT"); 
static boymue::JsEngine* s_engine;

void BoymueOnLoadWin::initWindow(HWND hwnd, int width, int height) {
  
  boymue::PaintContextWin* painter = new boymue::PaintContextWin();
  painter->initContext(hwnd, width, height);
  UIRuntime* uiRuntime = new UIRuntime(painter, width, height);
  boymue::BoymueAppInfo* info = new boymue::BoymueAppInfo();
  info->appName = "example";
  boymue::BoymueApplication* app = new boymue::BoymueApplication(info);
  s_uiRuntime = uiRuntime;
  s_domRepaintApp = app;
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
  app->getUITaskRunner().postTask([=] {
      s_uiRuntime->repaint(); 
  });
}
