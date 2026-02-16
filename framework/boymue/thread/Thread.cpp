#include "Thread.h"

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
static void StartThread(void* ptr)
{
    static_cast<Thread*>(ptr)->run();
}

Thread::Thread(const String& name)
    : m_name(name)
{
}

Thread::~Thread() {
}

void Thread::join() {
    m_thread->join();
}

void Thread::start() 
{
    m_thread = std::make_unique<std::thread>(&StartThread, this);
}

}