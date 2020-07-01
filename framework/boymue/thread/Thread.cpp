#include "Thread.h"
#include <thread>

namespace boymue {
void Thread::start() 
{
    std::thread th(&Thread::run, this);
    th.detach();
}
}