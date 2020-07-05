#ifndef Thread_h

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
class Thread {
public:
    void start();
    virtual void run() = 0;

private:
    static void startThread(void* ptr);
};
}
#endif // !Thread_h
