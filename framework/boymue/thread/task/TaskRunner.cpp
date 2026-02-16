// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05

#include "TaskRunner.h"

namespace boymue {
TaskRunner::TaskRunner()
    : m_order(0)
    , m_status(TaskRunner::kWaiting)
{
}

void TaskRunner::postTask(const closure& task)
{
    std::lock_guard guard(m_mutex);
    m_queue.push({ ++m_order, task, TimePoint::now() });
    m_event.signal();
}

closure TaskRunner::getInvocation()
{
    std::lock_guard guard(m_mutex);
    if (!m_queue.size()) {
        return nullptr;
    }

    const Task& task = m_queue.top();
    closure invocation = task.getTask();
    m_queue.pop();

    return invocation;
}

void TaskRunner::loop()
{
    while (m_status) {
        closure invocation = getInvocation();
        if (invocation) {
            m_status = kRunning;
            invocation();
            if (m_status) {
                m_status = kWaiting;
            }
        } else {
            m_event.wait();
        }
    }
}

void TaskRunner::terminate() {
    // 只允许在线程中修改status
    postTask([self = this] { self->m_status = TaskRunner::kExit; });
}

TaskRunner::TaskStatus TaskRunner::status() {
    return m_status;
}
}