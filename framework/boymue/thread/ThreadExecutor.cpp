// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.23

#include "ThreadExecutor.h"
#include "TaskThread.h"

namespace boymue {
class ThreadExecutorDefaultImpl : public ThreadExecutor {
public:
    ThreadExecutorDefaultImpl(const std::string& name)
        : m_thread(name)
    {
    }

    void submitTask(const closure& task) 
    {
        m_thread.getTaskRunner().postTask(task);
    }

private:
    TaskThread m_thread;
};

ThreadExecutor* ThreadExecutor::createDefault(const std::string& name) {
    return new ThreadExecutorDefaultImpl(name);
}
}
