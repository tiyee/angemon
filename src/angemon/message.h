#ifndef MESSAGE_H
#define MESSAGE_H
#include <queue>
#include <mutex>
using std::queue;
using std::mutex;

namespace angemon  {
template<class T>
class Message{
private:
    queue<T> _queue;
    mutable mutex _mutex;
public:
    Message& operator = (const Message&) = delete;
    Message(const Message& mq) = delete;
    void push(T msg);
    T pop();
    size_t Size();
    bool Empty();

};
}

#endif // MESSAGE_H
