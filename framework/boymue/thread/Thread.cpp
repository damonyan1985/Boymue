#include "Thread.h"

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
Thread::Thread(const std::string& name)
    : m_name(name)
{
}

Thread::~Thread() {
    m_thread->join();
}

void Thread::start() 
{
    m_thread = std::make_unique<std::thread>(&Thread::startThread, this);
}

void Thread::startThread(void* ptr)
{
    static_cast<Thread*>(ptr)->run();
}
}