// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.08

#ifndef AndroidEnvironmentGL_h
#define AndroidEnvironmentGL_h

#include <EGL/egl.h>

namespace boymue {

class AndroidEnvironmentGL {
public:
    // MakeRefCounted
    AndroidEnvironmentGL();

    // MakeRefCounted
    ~AndroidEnvironmentGL();

public:
    bool IsValid() const;

    EGLDisplay Display() const;

private:
    EGLDisplay display_;
    bool valid_;
};

} // namespace boymue

#endif // AndroidEnvironmentGL_h
