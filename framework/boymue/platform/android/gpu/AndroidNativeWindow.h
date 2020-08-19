// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.09

#ifndef AndroidNativeWindow_h
#define AndroidNativeWindow_h

#include "skia/include/core/SkSize.h"
#include <android/native_window.h>

namespace boymue {

class AndroidNativeWindow {
public:
    using Handle = ANativeWindow*;

    AndroidNativeWindow(Handle window);

    bool IsValid() const;

    Handle handle() const;

    SkISize GetSize() const;

private:
    Handle window_;
    
    ~AndroidNativeWindow();
};

} // namespace boymue

#endif // AndroidNativeWindow_h
