#ifndef TaskRunner_h
#define TaskRunner_h

#include "Task.h"
#include "WaitEvent.h"
#include <queue>
#include <mutex>

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
using TaskQueue = std::priority_queue<Task, 
                                    std::deque<Task>, 
                                    std::greater<Task>>;

class TaskRunner {
public:
    TaskRunner();
    void postTask(const closure& task);

private:
    // Thread loop
    void loop();
    // guard queue
    mutable std::mutex m_mutex;
    // wait for task
    WaitEvent m_event;
    TaskQueue m_queue;
    size_t m_order;
    bool m_status;

    friend class TaskThread;
};
}
#endif