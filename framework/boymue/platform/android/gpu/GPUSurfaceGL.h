#ifndef GPUSurfaceGL_h
#define GPUSurfaceGL_h

#include "skia/include/core/SkSurface.h"
#include "skia/include/gpu/GrContext.h"
#include <memory>

namespace boymue {
class GPUSurfaceGL {
public:
    GPUSurfaceGL();

    // Creates a new GL surface reusing an existing GrContext.
    GPUSurfaceGL(GrContext* gr_context);

    // |Surface|
    ~GPUSurfaceGL();

    // |Surface|
    bool IsValid();

    // |Surface|
    SkMatrix GetRootTransformation() const;

    // |Surface|
    GrContext* GetContext();

private:
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