// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.23

#ifndef ThreadExecutor_h
#define ThreadExecutor_h

#include "Task.h"

namespace boymue {
class ThreadExecutor {
public:
    static ThreadExecutor* createDefault(const std::string& name);
    virtual void submitTask(const closure& task) = 0;
};
}

#endif