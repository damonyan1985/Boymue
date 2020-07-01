#ifndef Thread_h

namespace boymue {
class Thread {
public:
    void start();
    virtual void run() = 0;
};
}
#endif // !Thread_h
