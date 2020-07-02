#include "Thread.h"
#include <thread>

namespace boymue {
void Thread::start() 
{
    std::thread th(&Thread::startThread, this);
    th.detach();
}

void Thread::startThread(void* ptr)
{
    Thread* th = static_cast<Thread*>(ptr);
    th->run();
}
}