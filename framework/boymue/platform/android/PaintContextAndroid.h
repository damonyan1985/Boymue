// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#ifndef PaintContextAndroid_h
#define PaintContextAndroid_h

#include <android/native_window_jni.h>

#include "AndroidContextGL.h"
#include "PaintContext.h"
#include "skia/include/core/SkSurface.h"
#include "skia/include/gpu/GrContext.h"

namespace boymue {
class PaintContextAndroid : public PaintContext {
 public:
  PaintContextAndroid();
  void initContext(AndroidNativeWindow* window, int width, int height);
  virtual void reset();
  virtual SkCanvas* canvas();
  virtual void submit();

 private:
  GrContext* m_context;
  AndroidContextGL* m_glContext;
  AndroidEGLSurface* m_eglSurface;
  SkSurface* m_surface;
};
}  // namespace boymue

#endif