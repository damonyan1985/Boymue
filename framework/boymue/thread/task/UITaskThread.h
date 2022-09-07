//
//  UITaskThread.hpp
//  core
//
//  Created by yanbo on 2022/9/5.
//

#ifndef UITaskThread_h
#define UITaskThread_h

#include "Thread.h"
#include "UITaskRunner.h"
#include "StringUtil.h"

namespace boymue {
class UITaskThread : public Thread {
public:
    UITaskThread(const String& name);
    virtual ~UITaskThread();

    UITaskRunner& getTaskRunner() const;

    virtual void run();
    virtual void terminate();

private:
    UITaskRunner* m_runner;
};
}

#endif /* UITaskThread_hpp */
