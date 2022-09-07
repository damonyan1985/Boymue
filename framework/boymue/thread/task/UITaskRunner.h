//
//  UITaskRunner.hpp
//  core
//
//  Created by yanbo on 2022/9/5.
//

#ifndef UITaskRunner_h
#define UITaskRunner_h

#include "TimePoint.h"

namespace boymue {
class UITaskRunner {
public:
    static UITaskRunner* createRunner();
    
    virtual ~UITaskRunner() {};
    
    virtual void loop() = 0;
    virtual void terminate() = 0;
    virtual void wakeUp(boymue::TimePoint time_point) = 0;
};

}

#endif /* UITaskRunner_hpp */
