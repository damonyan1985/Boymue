#include "Thread.h"
#include <thread>

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
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