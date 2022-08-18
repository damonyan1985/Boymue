//
//  PaintContextIOS.m
//  core
//
//  Created by yanbo on 2022/8/17.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/ES2/glext.h>
#include "PaintContextIOS.h"

namespace boymue {
PaintContextIOS::PaintContextIOS() {}
    
void PaintContextIOS::initContext(CAEAGLLayer* layer, int width, int height) {
    m_glContext = [BoymueIOSGLContext new];
    [m_glContext makeCurrent];
    
    //set up color render buffer
    glGenRenderbuffers(1, &m_colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_colorbuffer);
    [m_glContext attachLayer:layer];
    
    //set up frame buffer
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorbuffer);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorbuffer);
    
    //check success
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"Failed to make complete framebuffer object: %i", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }

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

    desc.fRenderTargetHandle = m_framebuffer;

    auto render_target_ =
        m_context->textureProvider()->wrapBackendRenderTarget(desc);

    SkSurfaceProps surface_props(
        SkSurfaceProps::InitType::kLegacyFontHost_InitType);

    m_surface = SkSurface::NewRenderTargetDirect(render_target_, &surface_props);
    
    glViewport(0, 0, width, height);

    glClearColor(1.0f, 0, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);    
}
    
SkCanvas* PaintContextIOS::canvas() {
    return m_surface->getCanvas();
}
    
void PaintContextIOS::submit() {
    m_context->flush();
    const GLenum discards[] = {
          GL_DEPTH_ATTACHMENT,
          GL_STENCIL_ATTACHMENT,
    };

    glDiscardFramebufferEXT(GL_FRAMEBUFFER, sizeof(discards) / sizeof(GLenum), discards);
    glBindRenderbuffer(GL_RENDERBUFFER, m_colorbuffer);
    [m_glContext swapBuffers];
}

void PaintContextIOS::reset() {}
}
