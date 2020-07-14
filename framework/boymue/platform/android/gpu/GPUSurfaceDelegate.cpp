// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.14

#include "GPUSurfaceDelegate.h"

#include "skia/include/gpu/gl/GrGLAssembleInterface.h"

namespace flutter {

GPUSurfaceDelegate::~GPUSurfaceDelegate() = default;

bool GPUSurfaceDelegate::GLContextFBOResetAfterPresent() const
{
    return false;
}

bool GPUSurfaceDelegate::SurfaceSupportsReadback() const
{
    return true;
}

SkMatrix GPUSurfaceDelegate::GLContextSurfaceTransformation() const
{
    SkMatrix matrix;
    matrix.setIdentity();
    return matrix;
}

static sk_sp<const GrGLInterface> CreateGLInterface(
    GPUSurfaceGLDelegate::GLProcResolver proc_resolver)
{
    if (proc_resolver == nullptr) {
        // If there is no custom proc resolver, ask Skia to guess the native
        // interface. This often leads to interesting results on most platforms.
        return GrGLMakeNativeInterface();
    }

    struct ProcResolverContext {
        GPUSurfaceGLDelegate::GLProcResolver resolver;
    };

    ProcResolverContext context = { proc_resolver };

    GrGLGetProc gl_get_proc = [](void* context,
                                  const char gl_proc_name[]) -> GrGLFuncPtr {
        auto proc_resolver_context = reinterpret_cast<ProcResolverContext*>(context);
        return reinterpret_cast<GrGLFuncPtr>(
            proc_resolver_context->resolver(gl_proc_name));
    };

    // glGetString indicates an OpenGL ES interface.
    if (IsProcResolverOpenGLES(proc_resolver)) {
        return GrGLMakeAssembledGLESInterface(&context, gl_get_proc);
    }

    // Fallback to OpenGL.
    if (auto interface = GrGLMakeAssembledGLInterface(&context, gl_get_proc)) {
        return interface;
    }

    //FML_LOG(ERROR) << "Could not create a valid GL interface.";
    return nullptr;
}

const GrGLInterface* GPUSurfaceGLDelegate::GetGLInterface() const
{
    return CreateGLInterface(GetGLProcResolver());
}

const GrGLInterface* GPUSurfaceGLDelegate::GetDefaultPlatformGLInterface()
{
    return CreateGLInterface(nullptr);
}

} // namespace flutter
