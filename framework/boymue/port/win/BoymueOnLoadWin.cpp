#include "BoymueOnLoadWin.h"
#include "Thread.h"
#include <stdio.h>

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
