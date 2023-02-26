// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.23

#ifndef ThreadExecutor_h
#define ThreadExecutor_h

#include "TaskThread.h"

namespace boymue {
class ThreadExecutor {
public:
    virtual ~ThreadExecutor() {};
    
    static ThreadExecutor* createDefault(const String& name);
    // single task thread 
    static ThreadExecutor* createSingleTaskExecutor(TaskThread* thread);
    virtual void submitTask(const closure& task) = 0;
};
}

#endif
