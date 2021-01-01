#ifndef TaskThread_h
#define TaskThread_h

#include "Thread.h"
#include "TaskRunner.h"

namespace boymue {
class TaskThread : public Thread {
public:
    TaskThread();
    virtual ~TaskThread();

    TaskRunner& getTaskRunner() const;

    virtual void run();

private:
    mutable TaskRunner m_runner;
};
}
#endif

