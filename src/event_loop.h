//
// Created by tiyee on 2021/8/25.
//

#ifndef ANGEMON_EVENT_LOOP_H
#define ANGEMON_EVENT_LOOP_H

#include "socket_utils.h"

namespace angemon {
    class Socket;

    typedef std::function<void()> Functor;
    typedef std::unordered_map<int, Socket *> SocketMap;

    class EventLoop {
    public:
        EventLoop();

        ~EventLoop();

        void Start();

        void Stop() {
            _stop = true;
        }


        bool IsStop() const {
            return _stop;
        }

        void SetThreadId(tid_t tid) {
            _thread_id = tid;
        }


        void AddSocketEvent(int fd, uint8_t socket_event, Socket *pSocket);

        void RemoveSocketEvent(int fd, uint8_t socket_event, Socket *pSocket);


        void runInLoop(const Functor &cb);


    private:
        void ReadWakeupData();

        bool IsInLoopThread() { return std::this_thread::get_id() == _thread_id; }

        void Wakeup();

        void queueInLoop(const Functor &cb);

        void doPendingFunctors();

        Socket *FindSocket(int fd) const;

        int poll(size_t ms);


    private:
        int _event_fd;
        tid_t _thread_id;
        bool _stop = false;
        int _wakeup_fds[2]{-1, -1};
        std::mutex _mutex;
        std::vector<Functor> _pendingFunctors;
        bool _functorPending;


        SocketMap _sockets;


    };
}


#endif //ANGEMON_EVENT_LOOP_H
