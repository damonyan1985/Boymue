// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PlatformViewAndroid_h
#define PlatformViewAndroid_h

#include "PlatformViewAndroid.h"
#include <memory>

namespace boymue {

class AndroidShellHolder {
public:
    AndroidShellHolder();

    ~AndroidShellHolder();

    PlatformViewAndroid* GetPlatformView();

private:
    PlatformViewAndroid* platform_view_;
};

} // namespace flutter

#endif // FLUTTER_SHELL_PLATFORM_ANDROID_ANDROID_SHELL_HOLDER_H_
