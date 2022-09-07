//
//  UITaskRunnerIOS.hpp
//  core
//
//  Created by yanbo on 2022/9/5.
//

#ifndef UITaskRunnerIOS_h
#define UITaskRunnerIOS_h

#include "UITaskRunner.h"

#include <CoreFoundation/CFRunLoop.h>
#include <Foundation/Foundation.h>

#include <atomic>

namespace boymue {
class UITaskRunnerIOS : public UITaskRunner {
public:
    UITaskRunnerIOS();
    virtual ~UITaskRunnerIOS();
    
    static CFStringRef kMessageLoopCFRunLoopMode;
    
private:
    virtual void loop() override;
    virtual void terminate() override;
    virtual void wakeUp(boymue::TimePoint time_point) override;
    
    static void onTimerFire(CFRunLoopTimerRef timer, UITaskRunnerIOS* runner);
    
    // 判断runner是否执行
    std::atomic_bool m_runing;
    CFRunLoopTimerRef m_timer;
    CFRunLoopRef m_loop;
};
}

#endif /* UITaskRunnerIOS_hpp */
