#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

#include"base_loop.h"
#include"base_socket.h"
#include "request.cpp"

using angemon::base::LoopImp;
using angemon::event::Event;
using angemon::base::SocketImp;
namespace angemon {
    class Socket : public SocketImp {
    private:
        const char *_ip;
        const unsigned int _port;
        int _lfd = -1;
        const int MAX_EVENT_COUNT = 5000;
        const int MAX_RECV_BUFF = 2048;
        LoopImp *_loop;
    public:
        Socket(const char *ip, unsigned int port, LoopImp *&loop_);

        Socket(const Socket &) = delete;

        Socket &operator=(const Socket &) = delete;

        static void setNonBlock(int fd_);

        virtual int accept(int fd_);

        int _create()final ;

        int _read(Event *&e)final ;

        int _write(Event *&e)final ;

        void _close(Event *&e)final ;

        void _start()final ;

    };

}


#endif // UNIX_SOCKET_H
