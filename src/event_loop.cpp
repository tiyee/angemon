//
// Created by tiyee on 2021/8/25.
//

#include "event_loop.h"

#ifdef __APPLE__

#include <sys/event.h>

#else

#include <sys/epoll.h>

#endif

angemon::EventLoop::EventLoop() : _stop(false), _functorPending(false) {
#ifdef __APPLE__
    _event_fd = kqueue();
#else
    _event_fd = epoll_create(1024);
#endif
    assert(_event_fd >= 0);
    _wakeup_fds[0] = _wakeup_fds[1] = -1;
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, _wakeup_fds) == 0);

    SetNonblock(_wakeup_fds[0], true);
    SetNonblock(_wakeup_fds[1], true);
}

angemon::EventLoop::~EventLoop() {
    LOG("~EventLoop");
}

void angemon::EventLoop::ReadWakeupData() {
    char buf[16];
    while (read(_wakeup_fds[0], buf, 16) >= 0);

}

int angemon::EventLoop::poll(size_t wait_timeout) {
#ifdef __APPLE__

    struct kevent events[1024];

    struct timespec timeout{};

    timeout.tv_sec = (long) wait_timeout / 1000;
    timeout.tv_nsec = ((long) wait_timeout % 1000) * 1000000;
    if (!IsStop()) {
        int nfds = ::kevent(_event_fd, nullptr, 0, events, 1024, &timeout);

        for (int i = 0; i < nfds; i++) {
            LOG("POLL n=%d i=%d", nfds, i);
            if (!events[i].udata) {
                ReadWakeupData();
                continue;
            }

            /*
             ** use handle instead of BaseSocket pointer **
             Because kevent will return two event(read and write) for the same socket,
             if in the process of the first event, the connection was deleted, then it will crash
             when process the second event
             */
            auto handle = (int) reinterpret_cast<long>(events[i].udata);
            Socket *pSocket = FindSocket(handle);
            if (!pSocket) {
                continue;
            }
            if (events[i].filter == EVFILT_READ) {
                pSocket->onRead();
            } else if (events[i].filter == EVFILT_WRITE) {
                pSocket->onWrite();
            }

        }

    }


#else


    struct epoll_event events[1024];
    if (!IsStop()) {
       int  nfds = epoll_wait(_event_fd, events, 1024, wait_timeout);
        for (int i = 0; i < nfds; i++) {
            Socket *pSocket = (Socket *) events[i].data.ptr;
            if (!pSocket) {
                ReadWakeupData();
                continue;
            }


            if (events[i].events & EPOLLIN) {
                pSocket->onRead();
            }

            if (events[i].events & EPOLLOUT) {
                pSocket->onWrite();
            }

            if (events[i].events & (EPOLLPRI | EPOLLERR | EPOLLHUP)) {
                pSocket->onClose();
            }

        }


    }

#endif
    return 0;
}

void angemon::EventLoop::Start() {
    LOG("EventLoop::Start %d", _thread_id);
    AddSocketEvent(_wakeup_fds[0], SOCKET_READ, nullptr);
    while (!IsStop()) {
        poll(_event_fd % 100 + 50);
        doPendingFunctors();
    }
}

void angemon::EventLoop::AddSocketEvent(int fd, uint8_t socket_event, angemon::Socket *pSocket) {
    //  LOG("EventLoop::AddSocketEvent fd=%d,tid=%d run=%d evt=%d",fd,_thread_id,std::this_thread::get_id(),socket_event);
    if (IsInLoopThread()) {
        //LOG("AddSocketEvent IsInLoopThread fd=%d,evt=%d",fd,socket_event);
#ifdef __APPLE__
        struct kevent ke{};
        void *udata = nullptr;
        if (pSocket) {
            udata = reinterpret_cast<void *>(pSocket->GetHandle());
        }
        if ((socket_event & SOCKET_READ) != 0) {
            EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, udata);
            kevent(_event_fd, &ke, 1, nullptr, 0, nullptr);
        }
        if ((socket_event & SOCKET_WRITE) != 0) {
            EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, udata);
            kevent(_event_fd, &ke, 1, nullptr, 0, nullptr);
        }


#else
        struct epoll_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLPRI | EPOLLERR | EPOLLHUP;
        ev.data.ptr = pSocket;
        if (epoll_ctl(_event_fd, EPOLL_CTL_ADD, fd, &ev) != 0) {
            LOG_ERR("epoll_ctl() failed, errno=%d err=%s", errno,strerror(errno));
        }

#endif
        if (((socket_event & SOCKET_ADD_CONN) != 0) && pSocket) {
            //printf("add conn: %d\n", pSocket->GetHandle());
            _sockets.insert(std::make_pair(pSocket->GetHandle(), pSocket));
        }
//        if (((socket_event & SOCKET_CONNECT_CB) != 0) && pSocket) {
//            pSocket->onConnect();
//        }
        // LOG("socket_event & SOCKET_WRITE fd=%d,evt=%d",fd,socket_event);
    } else {
        //  LOG("EventLoop::AddSocketEvent fd=%d,tid=%d run=%d evt=%d",fd,_thread_id,std::this_thread::get_id(),socket_event);
        queueInLoop([this, fd, socket_event, pSocket]() {
            this->AddSocketEvent(fd, socket_event, pSocket);
        });
        Wakeup();
    }
}

void angemon::EventLoop::RemoveSocketEvent(int fd, uint8_t socket_event, angemon::Socket *pSocket) {
    if (IsInLoopThread()) {
#ifdef __APPLE__
        struct kevent ke{};
        void *udata = nullptr;
        if (pSocket) {
            udata = reinterpret_cast<void *>(pSocket->GetHandle());
        }

        if ((socket_event & SOCKET_READ) != 0) {
            EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, udata);
            kevent(_event_fd, &ke, 1, nullptr, 0, nullptr);
        }

        if ((socket_event & SOCKET_WRITE) != 0) {
            EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, udata);
            kevent(_event_fd, &ke, 1, nullptr, 0, nullptr);
        }
#else
        if (epoll_ctl(_event_fd, EPOLL_CTL_DEL, fd, nullptr) != 0) {
            printf("epoll_ctl failed, errno=%d\n", errno);
        }
#endif

        if (((socket_event & SOCKET_DEL_CONN) != 0) && pSocket) {
            _sockets.erase(pSocket->GetHandle());
        }
    } else {
        queueInLoop([this, fd, socket_event, pSocket]() {
            printf("RemoveEvent queueInLoop FD=%d", fd);
            this->RemoveSocketEvent(fd, socket_event, pSocket);
        });
        Wakeup();
    }

}

void angemon::EventLoop::runInLoop(const angemon::Functor &cb) {
    if (IsInLoopThread()) {
        cb();
    } else {
        queueInLoop(cb);
    }
}

void angemon::EventLoop::Wakeup() {
    char buf = 0;
    send(_wakeup_fds[1], &buf, 1, 0);
}

void angemon::EventLoop::queueInLoop(const angemon::Functor &cb) {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _pendingFunctors.push_back(cb);
    }
    if (!IsInLoopThread() || _functorPending) {
        Wakeup();
    }
}

void angemon::EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    _functorPending = true;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        functors.swap(_pendingFunctors);
    }
    for (const auto &func:functors) {
        func();
    }
    _functorPending = false;
}

angemon::Socket *angemon::EventLoop::FindSocket(int handle) const {
    auto it = _sockets.find(handle);
    if (it != _sockets.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}
