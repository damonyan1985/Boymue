#include "TimePoint.h"

#include <chrono>

// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
TimePoint TimePoint::now() {
    const auto elapsed_time = std::chrono::steady_clock::now().time_since_epoch();
    return TimePoint(
        std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_time)
        .count());
}
}