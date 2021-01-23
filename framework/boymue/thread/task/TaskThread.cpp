#include "TaskThread.h"

namespace boymue {
TaskThread::TaskThread(const std::string& name)
    : Thread(name)
{
}

TaskThread::~TaskThread()
{
}

TaskRunner& TaskThread::getTaskRunner() const
{
    return m_runner;
}

void TaskThread::run()
{
    m_runner.loop();
}
}