// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.14

#ifndef GPUSurfaceGL_h
#define GPUSurfaceGL_h

#include "GPUSurfaceDelegate.h"
#include "skia/include/core/SkSurface.h"
#include "skia/include/gpu/GrContext.h"
#include <memory>

namespace boymue {
class GPUSurfaceGL {
public:
    // Creates a new GL surface reusing an existing GrContext.
    GPUSurfaceGL(GPUSurfaceDelegate* delegate);

    // |Surface|
    ~GPUSurfaceGL();

    // |Surface|
    bool IsValid();

    // |Surface|
    SkMatrix GetRootTransformation() const;

    // |Surface|
    GrContext* GetContext();

    void MakeRenderContextCurrent();

    void* AcquireFrame(const SkISize& size);

private:
    GPUSurfaceDelegate* delegate_;
    GrContext* context_;
    SkSurface* onscreen_surface_;
    bool context_owner_;

    GrRenderTarget* render_target_;
    bool valid_ = false;

    bool CreateOrUpdateSurfaces(const SkISize& size);

    SkSurface* AcquireRenderSurface(
        const SkISize& untransformed_size,
        const SkMatrix& root_surface_transformation);

    bool PresentSurface(SkCanvas* canvas);
};
}

#endif