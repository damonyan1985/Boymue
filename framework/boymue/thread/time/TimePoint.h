#ifndef TimePoint_h
#define TimePoint_h

#include <stdint.h>
#include <iosfwd>
#include "TimeDelta.h"

// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
class TimePoint {
public:
    // Default TimePoint with internal value 0 (epoch).
    constexpr TimePoint() = default;

    static TimePoint now();

    static constexpr TimePoint min() {
        return TimePoint(std::numeric_limits<int64_t>::min());
    }

    static constexpr TimePoint max() {
        return TimePoint(std::numeric_limits<int64_t>::max());
    }

    static constexpr TimePoint fromEpochDelta(TimeDelta ticks) {
        return TimePoint(ticks.toNanoseconds());
    }

    TimeDelta ToEpochDelta() const { return TimeDelta::fromNanoseconds(m_ticks); }

    // Compute the difference between two time points.
    TimeDelta operator-(TimePoint other) const {
        return TimeDelta::fromNanoseconds(m_ticks - other.m_ticks);
    }

    TimePoint operator+(TimeDelta duration) const {
        return TimePoint(m_ticks + duration.toNanoseconds());
    }
    TimePoint operator-(TimeDelta duration) const {
        return TimePoint(m_ticks - duration.toNanoseconds());
    }

    bool operator==(TimePoint other) const { return m_ticks == other.m_ticks; }
    bool operator!=(TimePoint other) const { return m_ticks != other.m_ticks; }
    bool operator<(TimePoint other) const { return m_ticks < other.m_ticks; }
    bool operator<=(TimePoint other) const { return m_ticks <= other.m_ticks; }
    bool operator>(TimePoint other) const { return m_ticks > other.m_ticks; }
    bool operator>=(TimePoint other) const { return m_ticks >= other.m_ticks; }

private:
    explicit constexpr TimePoint(int64_t ticks) : m_ticks(ticks) {}

    int64_t m_ticks = 0;
};
}
#endif

