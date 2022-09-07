//
//  UITaskRunnerIOS.cpp
//  core
//
//  Created by yanbo on 2022/9/5.
//

#include "UITaskRunnerIOS.h"

namespace boymue {
static constexpr CFTimeInterval kDistantFuture = 1.0e10;
CFStringRef UITaskRunnerIOS::kMessageLoopCFRunLoopMode = CFSTR("boymueUITaskRunner");

UITaskRunnerIOS::UITaskRunnerIOS()
    : m_runing(false)
    , m_loop((CFRunLoopRef)CFRetain(CFRunLoopGetCurrent())) {
        
    CFRunLoopTimerContext timer_context = {
          .info = this,
    };
    m_timer = CFRunLoopTimerCreate(kCFAllocatorDefault, kDistantFuture /* fire date */,
                                   HUGE_VAL /* interval */, 0 /* flags */, 0 /* order */,
                                   reinterpret_cast<CFRunLoopTimerCallBack>(&UITaskRunnerIOS::onTimerFire)
                                   /* callout */,
                                   &timer_context /* context */);
    CFRunLoopAddTimer(m_loop, m_timer, kCFRunLoopCommonModes);
    CFRunLoopAddTimer(m_loop, m_timer, kMessageLoopCFRunLoopMode);
}

UITaskRunnerIOS::~UITaskRunnerIOS() {
    CFRelease(m_timer);
    CFRelease(m_loop);
}

void UITaskRunnerIOS::loop() {
    m_runing = true;

    while (m_runing) {
        int result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, kDistantFuture, YES);
        if (result == kCFRunLoopRunStopped || result == kCFRunLoopRunFinished) {
            // This handles the case where the loop is terminated using
            // CoreFoundation APIs.
            
            // TODO something
            m_runing = false;
        }
    }
}

void UITaskRunnerIOS::terminate() {
    m_runing = false;
    CFRunLoopStop(m_loop);
}

void UITaskRunnerIOS::wakeUp(boymue::TimePoint time_point) {
    CFRunLoopTimerSetNextFireDate(
          m_timer,
          CFAbsoluteTimeGetCurrent() + (time_point - boymue::TimePoint::now()).ToSecondsF());
}

void UITaskRunnerIOS::onTimerFire(CFRunLoopTimerRef timer, UITaskRunnerIOS* runner) {
    
}

UITaskRunner* UITaskRunner::createRunner() {
    return new UITaskRunnerIOS();
}
}
