#include "Task.h"

namespace boymue {
Task::Task(size_t order, const closure& func, TimePoint targetTime)
    : m_order(order)
    , m_func(func)
    , m_targetTime(targetTime)
{
}
Task::~Task()
{
}

bool Task::operator > (const Task& other) const
{
    if (m_targetTime == other.m_targetTime) {
        return m_order > other.m_order;
    }
    return m_targetTime > other.m_targetTime;
}

void Task::run() const
{
    m_func();
}
}