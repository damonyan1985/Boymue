// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.14

#include "GPUSurfaceGL.h"

#include "skia/include/core/SkColorFilter.h"
#include "skia/include/core/SkRect.h"
#include "skia/include/core/SkSurface.h"
#include "skia/include/gpu/gl/GrGLInterface.h"
//#include "skia/include/gpu/GrBackendSurface.h"
//#include "skia/include/gpu/GrContextOptions.h"

#include <GLES2/gl2.h>

// These are common defines present on all OpenGL headers. However, we don't
// want to perform GL header reasolution on each platform we support. So just
// define these upfront. It is unlikely we will need more. But, if we do, we can
// add the same here.
#define GPU_GL_RGBA8 0x8058
#define GPU_GL_RGBA4 0x8056
#define GPU_GL_RGB565 0x8D62

namespace boymue {

// Default maximum number of budgeted resources in the cache.
static const int kGrCacheMaxCount = 8192;

// Default maximum number of bytes of GPU memory of budgeted resources in the
// cache.
// The shell will dynamically increase or decrease this cache based on the
// viewport size, unless a user has specifically requested a size on the Skia
// system channel.
static const size_t kGrCacheMaxByteSize = 24 * (1 << 20);

GPUSurfaceGL::GPUSurfaceGL(GPUSurfaceDelegate* delegate)
    : delegate_(delegate)
{
    delegate_->GLContextMakeCurrent();
    // auto context_switch = delegate_->GLContextMakeCurrent();
    // if (!context_switch->GetResult()) {
    //     // FML_LOG(ERROR)
    //     //     << "Could not make the context current to setup the gr context.";
    //     return;
    // }

    // GrContextOptions options;

    // if (PersistentCache::cache_sksl()) {
    //     //FML_LOG(INFO) << "Cache SkSL";
    //     options.fShaderCacheStrategy = GrContextOptions::ShaderCacheStrategy::kSkSL;
    // }
    // PersistentCache::MarkStrategySet();
    // options.fPersistentCache = PersistentCache::GetCacheForProcess();

    // options.fAvoidStencilBuffers = true;

    // // To get video playback on the widest range of devices, we limit Skia to
    // // ES2 shading language when the ES3 external image extension is missing.
    // options.fPreferExternalImagesOverES3 = true;

    // // TODO(goderbauer): remove option when skbug.com/7523 is fixed.
    // // A similar work-around is also used in shell/common/io_manager.cc.
    // options.fDisableGpuYUVConversion = true;

    // auto context = GrContext::MakeGL(delegate_->GetGLInterface(), options);

    // if (context == nullptr) {
    //     //FML_LOG(ERROR) << "Failed to setup Skia Gr context.";
    //     return;
    // }

    // context_ = std::move(context);

    //const GrGLInterface* fCurIntf = GrGLCreateNativeInterface();
    context_ = GrContext::Create(kOpenGL_GrBackend, reinterpret_cast<GrBackendContext>(delegate_->GetGLInterface()));

    context_->setResourceCacheLimits(kGrCacheMaxCount, kGrCacheMaxByteSize);

    // GrBackendRenderTargetDesc desc;
    // desc.fWidth = SkScalarRoundToInt(720);
    // desc.fHeight = SkScalarRoundToInt(1280);
    // desc.fConfig = kSkia8888_GrPixelConfig;
    // desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
    // desc.fSampleCnt = 1;
    // desc.fStencilBits = 8;

    // GLint buffer;
    // glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
    // desc.fRenderTargetHandle = buffer;

    // render_target_ = context_->textureProvider()->wrapBackendRenderTarget(desc);

    //delegate_->GLContextClearCurrent();
}

GPUSurfaceGL::~GPUSurfaceGL()
{
    // if (!valid_) {
    //     return;
    // }
    // auto context_switch = delegate_->GLContextMakeCurrent();
    // if (!context_switch->GetResult()) {
    //     // FML_LOG(ERROR) << "Could not make the context current to destroy the "
    //     //                   "GrContext resources.";
    //     return;
    // }

    // onscreen_surface_ = nullptr;
    // if (context_owner_) {
    //     context_->releaseResourcesAndAbandonContext();
    // }
    // context_ = nullptr;

    delegate_->GLContextClearCurrent();
}

// |Surface|
bool GPUSurfaceGL::IsValid()
{
    return valid_;
}

// static SkColorType FirstSupportedColorType(GrContext* context,
//     GrGLenum* format)
// {
// #define RETURN_IF_RENDERABLE(x, y)                   \
//     if (context->colorTypeSupportedAsSurface((x))) { \
//         *format = (y);                               \
//         return (x);                                  \
//     }
//     RETURN_IF_RENDERABLE(kRGBA_8888_SkColorType, GPU_GL_RGBA8);
//     RETURN_IF_RENDERABLE(kARGB_4444_SkColorType, GPU_GL_RGBA4);
//     RETURN_IF_RENDERABLE(kRGB_565_SkColorType, GPU_GL_RGB565);
//     return kUnknown_SkColorType;
// }

static SkSurface* WrapOnscreenSurface(GrContext* context,
    const SkISize& size,
    intptr_t fbo)
{
    // GrGLenum format;
    // const SkColorType color_type = FirstSupportedColorType(context, &format);

    // GrGLFramebufferInfo framebuffer_info = {};
    // framebuffer_info.fFBOID = static_cast<GrGLuint>(fbo);
    // framebuffer_info.fFormat = format;

    // GrBackendRenderTarget render_target(size.width(), // width
    //     size.height(), // height
    //     0, // sample count
    //     0, // stencil bits (TODO)
    //     framebuffer_info // framebuffer info
    // );

    // sk_sp<SkColorSpace> colorspace = SkColorSpace::MakeSRGB();

    SkSurfaceProps surface_props(
        SkSurfaceProps::InitType::kLegacyFontHost_InitType);

    // return SkSurface::MakeFromBackendRenderTarget(
    //     context, // gr context
    //     render_target, // render target
    //     GrSurfaceOrigin::kBottomLeft_GrSurfaceOrigin, // origin
    //     color_type, // color type
    //     colorspace, // colorspace
    //     &surface_props // surface properties
    // );

    GrBackendRenderTargetDesc desc;
    desc.fWidth = SkScalarRoundToInt(720);
    desc.fHeight = SkScalarRoundToInt(1280);
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
    desc.fSampleCnt = 1;
    desc.fStencilBits = 8;

    GLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
    desc.fRenderTargetHandle = buffer;

    auto render_target_ = context->textureProvider()->wrapBackendRenderTarget(desc);

    // SkSurfaceProps surface_props(
    //     SkSurfaceProps::InitType::kLegacyFontHost_InitType);

    return SkSurface::NewRenderTargetDirect(render_target_, &surface_props);
}

bool GPUSurfaceGL::CreateOrUpdateSurfaces(const SkISize& size)
{
    if (onscreen_surface_ != nullptr && size == SkISize::Make(onscreen_surface_->width(), onscreen_surface_->height())) {
        // Surface size appears unchanged. So bail.
        return true;
    }

    // We need to do some updates.
    //TRACE_EVENT0("flutter", "UpdateSurfacesSize");

    // Either way, we need to get rid of previous surface.
    onscreen_surface_ = nullptr;

    if (size.isEmpty()) {
        //FML_LOG(ERROR) << "Cannot create surfaces of empty size.";
        return false;
    }

    SkSurface* onscreen_surface = WrapOnscreenSurface(context_, // GL context
        size, // root surface size
        delegate_->GLContextFBO() // window FBO ID
    );

    if (onscreen_surface == nullptr) {
        // If the onscreen surface could not be wrapped. There is absolutely no
        // point in moving forward.
        //FML_LOG(ERROR) << "Could not wrap onscreen surface.";
        return false;
    }

    onscreen_surface_ = std::move(onscreen_surface);

    return true;
}

// |Surface|
SkMatrix GPUSurfaceGL::GetRootTransformation() const
{
    return delegate_->GLContextSurfaceTransformation();
}

// |Surface|
void* GPUSurfaceGL::AcquireFrame(const SkISize& size)
{
    // if (!delegate_) {
    //     return nullptr;
    // }

    // delegate_->GLContextMakeCurrent();
    // //auto context_switch = delegate_->GLContextMakeCurrent();
    // // if (!context_switch->GetResult()) {
    // //     // FML_LOG(ERROR)
    // //     //     << "Could not make the context current to acquire the frame.";
    // //     return nullptr;
    // // }

    // // TODO(38466): Refactor GPU surface APIs take into account the fact that an
    // // external view embedder may want to render to the root surface.
    // // if (!render_to_surface_) {
    // //     return std::make_unique<SurfaceFrame>(
    // //         nullptr, true, [](const SurfaceFrame& surface_frame, SkCanvas* canvas) {
    // //             return true;
    // //         });
    // // }

    // const auto root_surface_transformation = GetRootTransformation();

    // SkSurface* surface = AcquireRenderSurface(size, root_surface_transformation);

    // if (!surface) {
    //     return nullptr;
    // }

    // surface->getCanvas()->setMatrix(root_surface_transformation);
    // // SurfaceFrame::SubmitCallback submit_callback =
    // //     [weak = weak_factory_.GetWeakPtr()](const SurfaceFrame& surface_frame,
    // //         SkCanvas* canvas) {
    // //         return weak ? weak->PresentSurface(canvas) : false;
    // //     };

    // // return std::make_unique<SurfaceFrame>(
    // //     surface, delegate_->SurfaceSupportsReadback(), submit_callback,
    // //     std::move(context_switch));
}

bool GPUSurfaceGL::PresentSurface(SkCanvas* canvas)
{
    // if (delegate_ == nullptr || canvas == nullptr || context_ == nullptr) {
    //     return false;
    // }
    // {
    //     //TRACE_EVENT0("flutter", "SkCanvas::Flush");
    //     onscreen_surface_->getCanvas()->flush();
    // }

    // if (!delegate_->GLContextPresent()) {
    //     return false;
    // }

    // if (delegate_->GLContextFBOResetAfterPresent()) {
    //     auto current_size = SkISize::Make(onscreen_surface_->width(), onscreen_surface_->height());

    //     // The FBO has changed, ask the delegate for the new FBO and do a surface
    //     // re-wrap.
    //     auto new_onscreen_surface = WrapOnscreenSurface(context_, // GL context
    //         current_size, // root surface size
    //         delegate_->GLContextFBO() // window FBO ID
    //     );

    //     if (!new_onscreen_surface) {
    //         return false;
    //     }

    //     onscreen_surface_ = new_onscreen_surface;
    // }

    return true;
}

SkSurface* GPUSurfaceGL::AcquireRenderSurface(
    const SkISize& untransformed_size,
    const SkMatrix& root_surface_transformation)
{
    // const auto transformed_rect = root_surface_transformation.mapRect(
    //     &SkRect::MakeWH(untransformed_size.width(), untransformed_size.height()));

    // const auto transformed_size = SkISize::Make(transformed_rect.width(), transformed_rect.height());

    // if (!CreateOrUpdateSurfaces(transformed_size)) {
    //     return nullptr;
    // }

    // return onscreen_surface_;

    return nullptr;
}

// |Surface|
GrContext* GPUSurfaceGL::GetContext()
{
    return context_;
}

// |Surface|
void GPUSurfaceGL::MakeRenderContextCurrent()
{
    return delegate_->GLContextMakeCurrent();
}

} // namespace boymue
