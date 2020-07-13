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
#include "AndroidEnvironmentGL.h"

namespace boymue {

class AndroidSurfaceGL {
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

    // |GPUSurfaceGLDelegate|
    void GLContextMakeCurrent();

    // |GPUSurfaceGLDelegate|
    bool GLContextClearCurrent();

    // |GPUSurfaceGLDelegate|
    bool GLContextPresent();

    // |GPUSurfaceGLDelegate|
    intptr_t GLContextFBO() const;

private:
    const std::shared_ptr<AndroidContextGL> android_context_;

    AndroidNativeWindow* native_window_;
    std::unique_ptr<AndroidEGLSurface> onscreen_surface_;
    std::unique_ptr<AndroidEGLSurface> offscreen_surface_;
};

} // namespace flutter

#endif // FLUTTER_SHELL_PLATFORM_ANDROID_ANDROID_SURFACE_GL_H_
