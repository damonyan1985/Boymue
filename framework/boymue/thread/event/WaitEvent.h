#ifndef WaitEvent_h
#define WaitEvent_h

#include "TimeDelta.h"

#include <condition_variable>
#include <mutex>

// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
class WaitEvent final {
public:
    WaitEvent();
    ~WaitEvent();

    void signal();
    void reset();
    void wait();
    bool waitTimeout(TimeDelta timeout);

private:
    std::condition_variable m_cv;
    std::mutex m_mutex;
    bool m_signaled;

};
}
#endif
