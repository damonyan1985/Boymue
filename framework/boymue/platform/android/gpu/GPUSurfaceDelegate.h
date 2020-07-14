// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.14

#ifndef GPUSurfaceDelegate_h
#define GPUSurfaceDelegate_h

#include "skia/include/core/SkMatrix.h"
#include "skia/include/gpu/gl/GrGLInterface.h"

namespace boymue {
class GPUSurfaceDelegate {
public:
    virtual ~GPUSurfaceDelegate();

    // Called to make the main GL context current on the current thread.
    virtual void GLContextMakeCurrent() = 0;

    // Called to clear the current GL context on the thread. This may be called on
    // either the GPU or IO threads.
    virtual bool GLContextClearCurrent() = 0;

    // Called to present the main GL surface. This is only called for the main GL
    // context and not any of the contexts dedicated for IO.
    virtual bool GLContextPresent() = 0;

    // The ID of the main window bound framebuffer. Typically FBO0.
    virtual intptr_t GLContextFBO() const = 0;

    // The rendering subsystem assumes that the ID of the main window bound
    // framebuffer remains constant throughout. If this assumption in incorrect,
    // embedders are required to return true from this method. In such cases,
    // GLContextFBO() will be called again to acquire the new FBO ID for rendering
    // subsequent frames.
    virtual bool GLContextFBOResetAfterPresent() const;

    // Indicates whether or not the surface supports pixel readback as used in
    // circumstances such as a BackdropFilter.
    virtual bool SurfaceSupportsReadback() const;

    // A transformation applied to the onscreen surface before the canvas is
    // flushed.
    virtual SkMatrix GLContextSurfaceTransformation() const;

    const GrGLInterface* GetGLInterface() const;

    // TODO(chinmaygarde): The presence of this method is to work around the fact
    // that not all platforms can accept a custom GL proc table. Migrate all
    // platforms to move GL proc resolution to the embedder and remove this
    // method.
    static const GrGLInterface* GetDefaultPlatformGLInterface();
};

} // namespace boymue

#endif // GPUSurfaceDelegate_h
