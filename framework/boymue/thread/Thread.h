#ifndef Thread_h

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
