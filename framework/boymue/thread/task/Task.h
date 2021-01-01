#ifndef Task_h
#define Task_h

#include "TimePoint.h"
#include <functional>

namespace boymue {
using closure = std::function<void()>;
class Task {
public:
    Task(size_t order, const closure& func, TimePoint targetTime);
    virtual ~Task();
    // Task execute unit
    virtual void run() const;
    // Task priority compare
    bool operator > (const Task& other) const;

private:
    size_t m_order;
    TimePoint m_targetTime;
    closure m_func;
};
}

#endif
