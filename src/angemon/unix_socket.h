#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

#include"loop.h"
#include"base_socket.h"
#include "request.cpp"
using angemon::base::Loop;
using angemon::event::Event;
namespace angemon {
    class UnixSocket : public base::BaseSocket {
    private:
        const char *_ip;
        const unsigned int _port;
        int _lfd = -1;
        const int MAX_EVENT_COUNT = 5000;
        const int MAX_RECV_BUFF = 2048;
        Loop *_loop;
    public:
        UnixSocket(const char *ip, unsigned int port, Loop *&loop_);

        UnixSocket(const UnixSocket &) = delete;

        UnixSocket &operator=(const UnixSocket &) = delete;

        static void setNonBlock(int fd_);

        int accept(int fd_);

        int _create();

        int _read(Event *&e);

        int _write(Event *&e);

        void _close(Event *&e);

        void _start();

    };

}


#endif // UNIX_SOCKET_H
