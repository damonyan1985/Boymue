#ifndef Thread_h
#define Thread_h

#include <thread>
#include "StringUtil.h"

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
class Thread {
public:
    Thread(const String& name);
    ~Thread();
    void start();
    void join();

    virtual void run() = 0;
    virtual void terminate() = 0;    

private:
    static void startThread(void* ptr);
    OwnerPtr<std::thread> m_thread;
    String m_name;
};
}
#endif // !Thread_h
