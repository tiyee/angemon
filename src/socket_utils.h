//
// Created by tiyee on 2021/8/25.
//

#ifndef ANGEMON_SOCKET_UTILS_H
#define ANGEMON_SOCKET_UTILS_H

#include <netinet/in.h>
#include <netinet/tcp.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "log.h"
#include "event_loop.h"
#include "socket.h"
#include "session.h"

namespace angemon {
    const int SOCKET_ERROR = -1;
    const int INVALID_SOCKET = -1;

    enum {
        SOCKET_STATE_IDLE,
        SOCKET_STATE_LISTENING,
        SOCKET_STATE_CONNECTING,
        SOCKET_STATE_CONNECTED,
        SOCKET_STATE_PEER_CLOSING,
        SOCKET_STATE_CLOSING
    };
    enum {
        SOCKET_READ = 0x1,
        SOCKET_WRITE = 0x2,
        SOCKET_EXCEP = 0x4,
        SOCKET_ALL = 0x7,
        SOCKET_CONNECT_CB = 0x8,
        SOCKET_ADD_CONN = 0x10,
        SOCKET_DEL_CONN = 0x20,
    };

    int Listen(const std::string &server_ip_, uint16_t port_);

    void SetNonblock(int fd, bool nonblock);

    void SetReuseAddr(int fd);

    void SetNoDelay(int fd);

    void SetAddr(const std::string &ip, uint16_t port, sockaddr_in *pAddr);

    void GetBindAddr(int fd, std::string &bind_ip, uint16_t &bind_port);

    bool IsBlock(int error_code);

    void SetFastAck(int fd);

    int Read(void *buff, size_t len);

    int Write(void *buff, size_t len);
}


#endif //ANGEMON_SOCKET_UTILS_H
