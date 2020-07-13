// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.10.

#include "AndroidSurfaceGL.h"

#include <utility>

namespace boymue {

AndroidSurfaceGL::AndroidSurfaceGL(
    std::shared_ptr<AndroidContext> android_context)
    : android_context_(
        std::static_pointer_cast<AndroidContextGL>(android_context))
    , native_window_(nullptr)
    , onscreen_surface_(nullptr)
    , offscreen_surface_(nullptr)
{
    // Acquire the offscreen surface.
    offscreen_surface_ = android_context_->CreateOffscreenSurface();
    if (!offscreen_surface_->IsValid()) {
        offscreen_surface_ = nullptr;
    }
}

AndroidSurfaceGL::~AndroidSurfaceGL() = default;

void AndroidSurfaceGL::TeardownOnScreenContext()
{
    android_context_->ClearCurrent();
}

bool AndroidSurfaceGL::IsValid() const
{
    return offscreen_surface_ && android_context_->IsValid();
}

// std::unique_ptr<Surface> AndroidSurfaceGL::CreateGPUSurface(
//     GrContext* gr_context)
// {
//     if (gr_context) {
//         return std::make_unique<GPUSurfaceGL>(sk_ref_sp(gr_context), this, true);
//     }
//     return std::make_unique<GPUSurfaceGL>(this, true);
// }

bool AndroidSurfaceGL::OnScreenSurfaceResize(const SkISize& size)
{
    if (size == onscreen_surface_->GetSize()) {
        return true;
    }

    android_context_->ClearCurrent();

    // Ensure the destructor is called since it destroys the `EGLSurface` before
    // creating a new onscreen surface.
    onscreen_surface_ = nullptr;
    onscreen_surface_ = android_context_->CreateOnscreenSurface(native_window_);
    if (!onscreen_surface_->IsValid()) {
        return false;
    }
    onscreen_surface_->MakeCurrent();
    return true;
}

bool AndroidSurfaceGL::ResourceContextMakeCurrent()
{
    return offscreen_surface_->MakeCurrent();
}

bool AndroidSurfaceGL::ResourceContextClearCurrent()
{
    return android_context_->ClearCurrent();
}

bool AndroidSurfaceGL::SetNativeWindow(
    AndroidNativeWindow* window)
{
    native_window_ = window;
    // Create the onscreen surface.
    onscreen_surface_ = android_context_->CreateOnscreenSurface(window);
    if (!onscreen_surface_->IsValid()) {
        return false;
    }
    return true;
}

// std::unique_ptr<GLContextResult> AndroidSurfaceGL::GLContextMakeCurrent()
// {
//     auto default_context_result = std::make_unique<GLContextDefaultResult>(
//         onscreen_surface_->MakeCurrent());
//     return std::move(default_context_result);
// }

bool AndroidSurfaceGL::GLContextClearCurrent()
{
    return android_context_->ClearCurrent();
}

bool AndroidSurfaceGL::GLContextPresent()
{
    return onscreen_surface_->SwapBuffers();
}

intptr_t AndroidSurfaceGL::GLContextFBO() const
{
    // The default window bound framebuffer on Android.
    return 0;
}

} // namespace boymue
