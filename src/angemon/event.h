#ifndef EVENT_H
#define EVENT_H

#include"context.h"
#include<functional>

using angemon::Context;
namespace angemon {
    namespace event {

        const int E_READ = 0x1;
        const int E_WRITE = 0x2;
        const int E_SIGNAL = 0x4;
        const int E_TIMEOUT = 0x8;
        const int E_EDGE = 0x10;/* edge triggered */
        const int E_ONCE = 0x20;/* one-time event */
        const int E_IN_REACTOR = 0x40;
        const int E_OUT_OF_TIMERHEAP = 0;

        struct Event {
            const int fd;
            int ev_flags;
            Context *ctx;
            void *data;
            std::function<int(Context *)> cb;
            Event(int fd_, short flag_) : fd(fd_), ev_flags(flag_), ctx(nullptr), data(nullptr) {};


        };
    }

}
#endif // EVENT_H
