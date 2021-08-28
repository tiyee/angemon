//
// Created by tiyee on 2021/8/25.
//

#include "socket_utils.h"

int angemon::Listen(const std::string &server_ip, uint16_t port) {

    int fd;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == INVALID_SOCKET) {
        LOG_ERR("socket failed, err_code=%d,err=%s", errno, strerror(errno))
        return fd;
    }
    SetReuseAddr(fd);
    SetNonblock(fd, true);

    sockaddr_in serv_addr{};

    SetAddr(server_ip, port, &serv_addr);

    int ret = ::bind(fd, (sockaddr *) &serv_addr, sizeof(serv_addr));
    if (ret == SOCKET_ERROR) {
        LOG_ERR("bind %s:%d failed, err_code=%d err=%s", server_ip.c_str(), port, errno, strerror(errno));
        close(fd);
        return fd;
    }
    ret = listen(fd, 1024);
    if (ret == SOCKET_ERROR) {
        LOG_ERR("listen failed, err_code=%d err=%s", errno, strerror(errno));
        close(fd);
        return fd;
    }

    return fd;

}

void angemon::SetNonblock(int fd, bool nonblock) {
    int ret = 0;
    int flags = fcntl(fd, F_GETFL);
    if (nonblock) {
        ret = fcntl(fd, F_SETFL, O_NONBLOCK | flags);
    } else {
        ret = fcntl(fd, F_SETFL, ~O_NONBLOCK & flags);
    }

    if (ret == SOCKET_ERROR) {
        LOG_ERR("SetNonblock failed, err_code=%d err=%s", errno, strerror(errno));
    }
}

void angemon::SetReuseAddr(int fd) {
    int reuse = 1;
    int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(reuse));
    if (ret == SOCKET_ERROR) {
        LOG_ERR("SetReuseAddr failed, err_code=%d err=%s", errno, strerror(errno));
    }
}

void angemon::SetNoDelay(int fd) {
    int nodelay = 1;
    int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &nodelay, sizeof(nodelay));
    if (ret == SOCKET_ERROR) {
        LOG_ERR("SetNoDelay failed, err_code=%d err=%s", errno, strerror(errno));
    }
}

void angemon::SetAddr(const std::string &ip, uint16_t port_, sockaddr_in *pAddr) {
    ::memset(pAddr, 0, sizeof(sockaddr_in));
    pAddr->sin_family = AF_INET;
    pAddr->sin_port = htons(port_);
    pAddr->sin_addr.s_addr = inet_addr(ip.c_str());
    if (pAddr->sin_addr.s_addr == INADDR_NONE) {
        hostent *host = gethostbyname(ip.c_str());
        if (host == nullptr) {
            LOG_ERR("gethostbyname failed, ip=%s\n", ip.c_str());
            return;
        }

        pAddr->sin_addr.s_addr = *(uint32_t *) host->h_addr;
    }
}

void angemon::GetBindAddr(int fd, std::string &bind_ip, uint16_t &bind_port) {
    struct sockaddr_in local_addr{};
    socklen_t len = sizeof(local_addr);
    getsockname(fd, (sockaddr *) &local_addr, &len);
    uint32_t ip = ntohl(local_addr.sin_addr.s_addr);
    char ip_str[64];
    snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d", ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
    bind_ip = std::string(ip_str);
    bind_port = ntohs(local_addr.sin_port);
}

bool angemon::IsBlock(int error_code) {
    return ((error_code == EINPROGRESS) || (error_code == EWOULDBLOCK));
}

void angemon::SetFastAck(int fd) {
#ifndef __APPLE__
    int quick_ack = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, (void *) &quick_ack, sizeof(quick_ack));
#endif
}