#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include"common.h"
#include"thread"
using std::vector;
using std::thread;
namespace angemon {
template<class T>
class ThreadPool{
    ThreadPool& operator = (const ThreadPool&) = delete;
    ThreadPool(const ThreadPool& other) = delete;
private:
    vector<thread> _workers;
    size_t _threads;
public:
    void run();
    void submit(T task_);
};
}

#endif // THREAD_POOL_H
