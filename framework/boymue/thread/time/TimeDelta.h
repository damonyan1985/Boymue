#ifndef TimeDelta_h
#define TimeDelta_h

#include <stdint.h>
#include <time.h>

#include <chrono>
#include <iosfwd>
#include <limits>

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
class TimeDelta {
public:
    constexpr TimeDelta() = default;

    static constexpr TimeDelta zero() { return TimeDelta(); }
    static constexpr TimeDelta min() {
        return TimeDelta(std::numeric_limits<int64_t>::min());
    }
    static constexpr TimeDelta max() {
        return TimeDelta(std::numeric_limits<int64_t>::max());
    }
    static constexpr TimeDelta fromNanoseconds(int64_t nanos) {
        return TimeDelta(nanos);
    }
    static constexpr TimeDelta fromMicroseconds(int64_t micros) {
        return fromNanoseconds(micros * 1000);
    }
    static constexpr TimeDelta FromMilliseconds(int64_t millis) {
        return fromMicroseconds(millis * 1000);
    }
    static constexpr TimeDelta fromSeconds(int64_t seconds) {
        return FromMilliseconds(seconds * 1000);
    }

    static constexpr TimeDelta fromSecondsF(double seconds) {
        return fromNanoseconds(seconds * (1000.0 * 1000.0 * 1000.0));
    }

    static constexpr TimeDelta FromMillisecondsF(double millis) {
        return fromNanoseconds(millis * (1000.0 * 1000.0));
    }

    constexpr int64_t toNanoseconds() const { return m_delta; }
    constexpr int64_t toMicroseconds() const { return toNanoseconds() / 1000; }
    constexpr int64_t toMilliseconds() const { return toMicroseconds() / 1000; }
    constexpr int64_t toSeconds() const { return toMilliseconds() / 1000; }

    constexpr double toNanosecondsF() const { return m_delta; }
    constexpr double toMicrosecondsF() const { return m_delta / 1000.0; }
    constexpr double toMillisecondsF() const {
        return m_delta / (1000.0 * 1000.0);
    }
    constexpr double ToSecondsF() const {
        return m_delta / (1000.0 * 1000.0 * 1000.0);
    }

    constexpr TimeDelta operator-(TimeDelta other) const {
        return TimeDelta::fromNanoseconds(m_delta - other.m_delta);
    }

    constexpr TimeDelta operator+(TimeDelta other) const {
        return TimeDelta::fromNanoseconds(m_delta + other.m_delta);
    }

    constexpr TimeDelta operator/(int64_t divisor) const {
        return TimeDelta::fromNanoseconds(m_delta / divisor);
    }

    constexpr int64_t operator/(TimeDelta other) const {
        return m_delta / other.m_delta;
    }

    constexpr TimeDelta operator*(int64_t multiplier) const {
        return TimeDelta::fromNanoseconds(m_delta * multiplier);
    }

    constexpr TimeDelta operator%(TimeDelta other) const {
        return TimeDelta::fromNanoseconds(m_delta % other.m_delta);
    }

    bool operator==(TimeDelta other) const { return m_delta == other.m_delta; }
    bool operator!=(TimeDelta other) const { return m_delta != other.m_delta; }
    bool operator<(TimeDelta other) const { return m_delta < other.m_delta; }
    bool operator<=(TimeDelta other) const { return m_delta <= other.m_delta; }
    bool operator>(TimeDelta other) const { return m_delta > other.m_delta; }
    bool operator>=(TimeDelta other) const { return m_delta >= other.m_delta; }

    static constexpr TimeDelta fromTimespec(struct timespec ts) {
        return TimeDelta::fromSeconds(ts.tv_sec) +
            TimeDelta::fromNanoseconds(ts.tv_nsec);
    }
    struct timespec ToTimespec() {
        struct timespec ts;
        constexpr int64_t kNanosecondsPerSecond = 1000000000ll;
        ts.tv_sec = static_cast<time_t>(toSeconds());
        ts.tv_nsec = m_delta % kNanosecondsPerSecond;
        return ts;
    }

private:
    // Private, use one of the FromFoo() types
    explicit constexpr TimeDelta(int64_t delta) : m_delta(delta) {}

    int64_t m_delta = 0;
};

}
#endif