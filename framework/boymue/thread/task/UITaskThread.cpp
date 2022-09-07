//
//  UITaskThread.cpp
//  core
//
//  Created by yanbo on 2022/9/5.
//

#include "UITaskThread.h"

namespace boymue {
UITaskThread::UITaskThread(const String& name)
    : Thread(name)
    , m_runner(UITaskRunner::createRunner())
{
}

UITaskThread::~UITaskThread()
{
    terminate();
    join();
}

UITaskRunner& UITaskThread::getTaskRunner() const
{
    return *m_runner;
}

void UITaskThread::run()
{
    m_runner->loop();
}

void UITaskThread::terminate()
{
    m_runner->terminate();
}
}
