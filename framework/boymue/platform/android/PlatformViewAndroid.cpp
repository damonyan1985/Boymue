// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.14

#include "PlatformViewAndroid.h"
#include "GPUSurfaceGL.h"

namespace boymue {
PlatformViewAndroid::PlatformViewAndroid()
{
    // 创建GL上下文环境
    std::shared_ptr<AndroidContextGL> android_context = std::make_shared<AndroidContextGL>(
        std::make_unique<AndroidEnvironmentGL>());

    // 创建surface
    m_surface
        = std::make_unique<AndroidSurfaceGL>(android_context);
}

// 设置AndroidNativeWindow
void PlatformViewAndroid::NotifyCreated(AndroidNativeWindow* native_window)
{
    // gpu线程执行
    m_surface->SetNativeWindow(native_window);
    //std::unique_ptr<Surface> surface = CreateRenderingSurface();
}

void PlatformViewAndroid::NotifySurfaceWindowChanged(
    AndroidNativeWindow* native_window)
{
}
void PlatformViewAndroid::NotifyChanged(const SkISize& size)
{
}

void PlatformViewAndroid::NotifyDestroyed()
{
}

std::unique_ptr<GPUSurfaceGL> PlatformViewAndroid::CreateRenderingSurface()
{
    if (!m_surface) {
        return nullptr;
    }
    return m_surface->CreateGPUSurface();
}
}