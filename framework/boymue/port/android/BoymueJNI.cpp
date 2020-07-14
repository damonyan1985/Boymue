// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.14
#include "AndroidShellHolder.h"
#include <android/native_window_jni.h>
#include <jni.h>

#define ANDROID_SHELL_HOLDER \
    (reinterpret_cast<AndroidShellHolder*>(shell_holder))

static void SurfaceCreated(JNIEnv* env,
    jobject jcaller,
    jlong shell_holder,
    jobject jsurface)
{
    // Note: This frame ensures that any local references used by
    // ANativeWindow_fromSurface are released immediately. This is needed as a
    // workaround for https://code.google.com/p/android/issues/detail?id=68174
    //fml::jni::ScopedJavaLocalFrame scoped_local_reference_frame(env);
    auto window = ANativeWindow_fromSurface(env, jsurface);
    ANDROID_SHELL_HOLDER->GetPlatformView()->NotifyCreated(window);
}