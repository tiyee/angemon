#include "linux_loop.h"
#include <sys/epoll.h>

namespace angemon {
    namespace linux {
        Loop::Loop() {
            _efd = epoll_create(20);
            if (-1 == crRet) {
                LOG_ERR("epoll create fail")
            }


        }

    }
}

