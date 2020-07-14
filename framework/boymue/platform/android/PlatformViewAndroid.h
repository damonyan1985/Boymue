// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.14

#ifndef PlatformViewAndroid_h
#define PlatformViewAndroid_h

#include "AndroidSurfaceGL.h"
#include <memory>

namespace boymue {
class GPUSurfaceGL;
class PlatformViewAndroid {
public:
    PlatformViewAndroid();
    ~PlatformViewAndroid();

    void NotifyCreated(AndroidNativeWindow* native_window);
    void NotifySurfaceWindowChanged(
        AndroidNativeWindow* native_window);
    void NotifyChanged(const SkISize& size);

    void NotifyDestroyed();

    std::unique_ptr<GPUSurfaceGL> CreateRenderingSurface();

private:
    std::unique_ptr<AndroidSurfaceGL> m_surface;
};

}

#endif