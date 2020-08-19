// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.10

#ifndef AndroidSurfaceGL_h
#define AndroidSurfaceGL_h

#include <jni.h>
#include <memory>

#include "AndroidContextGL.h"
#include "GPUSurfaceDelegate.h"

namespace boymue {
class GPUSurfaceGL;
class AndroidSurfaceGL : public GPUSurfaceDelegate {
public:
    AndroidSurfaceGL(std::shared_ptr<AndroidContextGL> android_context);

    ~AndroidSurfaceGL();

    // |AndroidSurface|
    bool IsValid() const;

    // |AndroidSurface|
    void TeardownOnScreenContext();

    // |AndroidSurface|
    bool OnScreenSurfaceResize(const SkISize& size);

    // |AndroidSurface|
    bool ResourceContextMakeCurrent();

    // |AndroidSurface|
    bool ResourceContextClearCurrent();

    // |AndroidSurface|
    bool SetNativeWindow(AndroidNativeWindow* window);

    GPUSurfaceGL* CreateGPUSurface();

    // |GPUSurfaceGLDelegate|
    virtual void GLContextMakeCurrent() override;

    // |GPUSurfaceGLDelegate|
    virtual bool GLContextClearCurrent() override;

    // |GPUSurfaceGLDelegate|
    virtual bool GLContextPresent() override;

    // |GPUSurfaceGLDelegate|
    virtual intptr_t GLContextFBO() const override;

private:
    const std::shared_ptr<AndroidContextGL> android_context_;

    AndroidNativeWindow* native_window_;
    AndroidEGLSurface* onscreen_surface_;
    AndroidEGLSurface* offscreen_surface_;
};

} // namespace flutter

#endif // FLUTTER_SHELL_PLATFORM_ANDROID_ANDROID_SURFACE_GL_H_
