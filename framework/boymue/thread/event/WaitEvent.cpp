#include "WaitEvent.h"
#include "TimePoint.h"

#include <errno.h>
#include <time.h>

// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
WaitEvent::WaitEvent()
    : m_signaled(false)
{
}

void WaitEvent::signal()
{
    std::scoped_lock locker(m_mutex);
    m_signaled = true;
    m_cv.notify_one();
}

void WaitEvent::reset()
{
    std::scoped_lock locker(m_mutex);
    m_signaled = false;
}

void WaitEvent::wait()
{
    std::unique_lock<std::mutex> locker(m_mutex);
    while (!m_signaled)
        m_cv.wait(locker);
    m_signaled = false;
}

bool WaitEvent::waitTimeout(TimeDelta timeout)
{
    std::unique_lock<std::mutex> locker(m_mutex);

    if (m_signaled) {
        m_signaled = false;
        return false;
    }

    // We may get spurious wakeups.
    TimeDelta wait_remaining = timeout;
    TimePoint start = TimePoint::now();
    while (true) {
        if (std::cv_status::timeout ==
            m_cv.wait_for(locker,
                std::chrono::nanoseconds(wait_remaining.toNanoseconds())))
            return true;  // Definitely timed out.

          // We may have been awoken.
        if (m_signaled)
            break;

        // Or the wakeup may have been spurious.
        TimePoint now = TimePoint::now();
        TimeDelta elapsed = now - start;
        // It's possible that we may have timed out anyway.
        if (elapsed >= timeout)
            return true;

        // Otherwise, recalculate the amount that we have left to wait.
        wait_remaining = timeout - elapsed;
    }

    m_signaled = false;
    return false;
}
}