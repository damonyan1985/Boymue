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
    GPUSurfaceGL(GrContext* gr_context, GPUSurfaceDelegate* delegate);

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
    // TODO(38466): Refactor GPU surface APIs take into account the fact that an
    // external view embedder may want to render to the root surface. This is a
    // hack to make avoid allocating resources for the root surface when an
    // external view embedder is present.
    const bool render_to_surface_;
    bool valid_ = false;

    bool CreateOrUpdateSurfaces(const SkISize& size);

    SkSurface* AcquireRenderSurface(
        const SkISize& untransformed_size,
        const SkMatrix& root_surface_transformation);

    bool PresentSurface(SkCanvas* canvas);
};
}

#endif