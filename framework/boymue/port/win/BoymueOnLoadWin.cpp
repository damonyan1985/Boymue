#include "BoymueOnLoadWin.h"
#include "Thread.h"
#include <stdio.h>

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
class TestThread : public boymue::Thread {
public:
    virtual void run() 
    {
        printf("just test");
        delete this;
    }
};

void BoymueOnLoadWin::init() {
    TestThread* thread = new TestThread();
    thread->start();
}
