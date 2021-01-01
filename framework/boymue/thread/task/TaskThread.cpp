#include "TaskThread.h"

namespace boymue {
TaskThread::TaskThread()
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