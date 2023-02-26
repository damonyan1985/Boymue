// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.23

#include "ThreadExecutor.h"
#include "TaskThread.h"

namespace boymue {
// TODO 后期修改为多线程处理
class ThreadExecutorDefaultImpl : public ThreadExecutor {
public:
    ThreadExecutorDefaultImpl(const String& name)
        : m_thread(name)
    {
        m_thread.start();
    }

    virtual void submitTask(const closure& task) override
    {
        m_thread.getTaskRunner().postTask(task);
    }

private:
    TaskThread m_thread;
};

class ThreadExecutorSingleTaskImpl : public ThreadExecutor {
public:
    ThreadExecutorSingleTaskImpl(TaskThread* thread)
        : m_thread(thread) { }

    virtual void submitTask(const closure& task) override
    {
        m_thread->getTaskRunner().postTask(task);
    }

private:
    TaskThread* m_thread;
};

ThreadExecutor* ThreadExecutor::createDefault(const String& name) {
    return new ThreadExecutorDefaultImpl(name);
}

ThreadExecutor* ThreadExecutor::createSingleTaskExecutor(TaskThread* thread) {
    return new ThreadExecutorSingleTaskImpl(thread);
}
}
