// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.08

#ifndef AndroidEGLSurface_h
#define AndroidEGLSurface_h

#include "AndroidEnvironmentGL.h"
#include "AndroidNativeWindow.h"
#include <memory>

namespace boymue {

//------------------------------------------------------------------------------
/// Holds an `EGLSurface` reference.
///
///
/// This can be used in conjuction to unique_ptr to provide better guarantees
/// about the lifespam of the `EGLSurface` object.
///
class AndroidEGLSurface {
public:
    AndroidEGLSurface(EGLSurface surface, EGLDisplay display, EGLContext context);
    ~AndroidEGLSurface();

    //----------------------------------------------------------------------------
    /// @return     Whether the current `EGLSurface` reference is valid. That is,
    /// if
    ///             the surface doesn't point to `EGL_NO_SURFACE`.
    ///
    bool IsValid() const;

    //----------------------------------------------------------------------------
    /// @brief      Binds the EGLContext context to the current rendering thread
    ///             and to the draw and read surface.
    ///
    /// @return     Whether the surface was made current.
    ///
    bool MakeCurrent();

    //----------------------------------------------------------------------------
    /// @brief      This only applies to on-screen surfaces such as those created
    ///             by `AndroidContextGL::CreateOnscreenSurface`.
    ///
    /// @return     Whether the EGL surface color buffer was swapped.
    ///
    bool SwapBuffers();

    //----------------------------------------------------------------------------
    /// @return     The size of an `EGLSurface`.
    ///
    SkISize GetSize() const;

private:
    const EGLSurface surface_;
    const EGLDisplay display_;
    const EGLContext context_;
};

//------------------------------------------------------------------------------
/// The Android context is used by `AndroidSurfaceGL` to create and manage
/// EGL surfaces.
///
/// This context binds `EGLContext` to the current rendering thread and to the
/// draw and read `EGLSurface`s.
///
class AndroidContextGL {
public:
    AndroidContextGL(std::unique_ptr<AndroidEnvironmentGL> environment);

    ~AndroidContextGL();

    //----------------------------------------------------------------------------
    /// @brief      Allocates an new EGL window surface that is used for on-screen
    ///             pixels.
    ///
    /// @return     The window surface.
    ///
    std::unique_ptr<AndroidEGLSurface> CreateOnscreenSurface(
        AndroidNativeWindow* window) const;

    //----------------------------------------------------------------------------
    /// @brief      Allocates an 1x1 pbuffer surface that is used for making the
    ///             offscreen current for texture uploads.
    ///
    /// @return     The pbuffer surface.
    ///
    std::unique_ptr<AndroidEGLSurface> CreateOffscreenSurface() const;

    //----------------------------------------------------------------------------
    /// @return     The Android environment that contains a reference to the
    /// display.
    ///
    AndroidEnvironmentGL* Environment() const;

    //----------------------------------------------------------------------------
    /// @return     Whether the current context is valid. That is, if the EGL
    /// contexts
    ///             were successfully created.
    ///
    bool IsValid() const;

    //----------------------------------------------------------------------------
    /// @return     Whether the current context was successfully clear.
    ///
    bool ClearCurrent();

private:
    std::unique_ptr<AndroidEnvironmentGL> environment_;
    EGLConfig config_;
    EGLContext context_;
    EGLContext resource_context_;
    bool valid_ = false;
};

} // namespace boymue

#endif // AndroidEGLSurface_h
