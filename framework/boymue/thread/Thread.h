#ifndef Thread_h
#define Thread_h

#include <memory>
#include <thread>
#include <string>

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
class Thread {
public:
    Thread(const std::string& name);
    ~Thread();
    void start();
    virtual void run() = 0;
    virtual void terminate() = 0;

private:
    static void startThread(void* ptr);
    std::unique_ptr<std::thread> m_thread;
    std::string m_name;
};
}
#endif // !Thread_h
