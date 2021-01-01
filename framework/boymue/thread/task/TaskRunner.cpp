#include "TaskRunner.h"

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
TaskRunner::TaskRunner()
    : m_order(0)
    , m_status(true)
{
}

void TaskRunner::postTask(const closure& task)
{
    std::lock_guard guard(m_mutex);
    m_queue.push({ ++m_order, task, TimePoint::now() });
    m_event.signal();
}

void TaskRunner::loop()
{
    while (m_status) {
        if (m_queue.size()) {
            std::lock_guard guard(m_mutex);
            const Task& task = m_queue.top();
            task.run();
            m_queue.pop();
        } else {
            m_event.wait();
        }
    }
}
}