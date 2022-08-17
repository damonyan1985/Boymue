//
//  PaintContextIOS.m
//  core
//
//  Created by yanbo on 2022/8/17.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/ES2/gl.h>
#include "PaintContextIOS.h"

namespace boymue {
PaintContextIOS::PaintContextIOS() {}
    
void PaintContextIOS::initContext(CAEAGLLayer* layer, int width, int height) {
    m_glContext = [BoymueIOSGLContext new];
    [m_glContext makeCurrent:layer];
    
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
    
SkCanvas* PaintContextIOS::canvas() {
    return m_surface->getCanvas();
}
    
void PaintContextIOS::submit() {
    m_context->flush();
    [m_glContext swapBuffers];
}

void PaintContextIOS::reset() {}
}
