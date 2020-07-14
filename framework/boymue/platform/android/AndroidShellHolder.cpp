// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define FML_USED_ON_EMBEDDER

#include "flutter/shell/platform/android/android_shell_holder.h"

#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>

#include <sstream>
#include <string>
#include <utility>

#include "AndroidShellHolder.h"

namespace boymue {
AndroidShellHolder::AndroidShellHolder()
{
}

AndroidShellHolder::~AndroidShellHolder()
{
}

PlatformViewAndroid* AndroidShellHolder::GetPlatformView()
{
    return platform_view_;
}
} // namespace boymue
