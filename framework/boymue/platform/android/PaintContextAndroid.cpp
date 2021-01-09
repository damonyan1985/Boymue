#include "PaintContextAndroid.h"

#include <GLES2/gl2.h>

#include "skia/include/gpu/gl/GrGLInterface.h"

namespace boymue {
PaintContextAndroid::PaintContextAndroid() {}

void PaintContextAndroid::initContext(AndroidNativeWindow* window, int width,
                                      int height) {
  m_glContext = new AndroidContextGL(new AndroidEnvironmentGL());
  m_eglSurface = m_glContext->CreateOnscreenSurface(window);
  m_eglSurface->MakeCurrent();

  glViewport(0, 0, width, height);

  glClearColor(1.0f, 0, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  const GrGLInterface* fCurIntf = GrGLCreateNativeInterface();
  m_context = GrContext::Create(kOpenGL_GrBackend,
                                reinterpret_cast<GrBackendContext>(fCurIntf));

  GrBackendRenderTargetDesc desc;
  desc.fWidth = width;
  desc.fHeight = height;
  desc.fConfig = kSkia8888_GrPixelConfig;
  desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
  desc.fSampleCnt = 1;
  desc.fStencilBits = 8;

  GLint buffer;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
  desc.fRenderTargetHandle = buffer;

  auto render_target_ =
      m_context->textureProvider()->wrapBackendRenderTarget(desc);

  SkSurfaceProps surface_props(
      SkSurfaceProps::InitType::kLegacyFontHost_InitType);

  m_surface = SkSurface::NewRenderTargetDirect(render_target_, &surface_props);
}

SkCanvas* PaintContextAndroid::canvas() { return m_surface->getCanvas(); }

void PaintContextAndroid::submit() {
  m_context->flush();
  m_eglSurface->SwapBuffers();
}

void PaintContextAndroid::reset() {}
}  // namespace boymue