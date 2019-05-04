#include "unix_socket.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
using angemon::event::E_READ;
using angemon::event::E_WRITE;
namespace angemon {

    UnixSocket::UnixSocket(const char *ip, unsigned int port, Loop *&loop_) : _ip(ip), _port(port), _loop(loop_) {};

    void UnixSocket::setNonBlock(int fd_) {
        auto currFlags = fcntl(fd_, F_GETFL, 0);
        if (currFlags < 0) {
            std::cerr << "fcntl get flags failed" << std::endl;

        }
        currFlags |= O_NONBLOCK;
        int r = fcntl(fd_, F_SETFL, currFlags);
        if (r < 0) {
            std::cerr << "fcntl set failed" << std::endl;
        };
    };

    int UnixSocket::_create() {

        int fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1) {
            LOG_ERR("socket() failed: errno=%d msg=%s", errno, strerror(errno))
            return -1;
        }

        LOG("socket() success fd=%l ", fd)
        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(_port);
        addr.sin_addr.s_addr = inet_addr(_ip);
        if (::bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {

            LOG_ERR("bind() failed: errno=%d msg=%s", errno, strerror(errno))
            return -1;
        }
        if (::listen(fd, 5) == -1) {
            LOG_ERR("listen() failed: errno=%d msg=%s", errno, strerror(errno))
            return -1;
        }
        setNonBlock(fd);

        LOG("listen() success fd=%d", fd)
        _lfd = fd;
        return fd;
    };

    int UnixSocket::accept(int fd_) {
        int client = ::accept(fd_, nullptr, nullptr);
        if (client == -1) {
            LOG_ERR("Accept failed. lfd=%d", fd_);
            return -1;
        }

        setNonBlock(client);
        LOG("accept() success fd=%d", client)
        auto e = new Event(client, E_READ);
        this->_loop->register_(e);
        return client;
    }

    int UnixSocket::_read(Event *&e) {
        auto fd_ = e->fd;
        std::string buff_;
        size_t realSize = 0;
        char buffer[1024] = {'\0'};
        while (true) {
            int ret = recv(fd_, buffer, 1024, 0);

            //从接收缓冲取数据成功
            if (ret > 0) {
                realSize += ret;
                for (int i = 0; i < ret; ++i) {
                    buff_.push_back(buffer[i]);
                }
                //不足1024，说明取完了
                if (ret < 1024) {
                    break;
                }
            }
            //发生错误
            if (ret == -1) {
                // EAGAIN/EWOULDBLOCK提示你的应用程序现在没有数据可读请稍后再试
                // EINTR指操作被中断唤醒，需要重新读
                if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                    break;
                } else if (errno == EINTR) {
                    continue;
                }
                    //异常断开情况
                else {
                    this->_close(e);
                    break;
                }
            }
            //接收到主动关闭请求
            if (ret == 0) {
                this->_close(e);
                break;
            }
        }

        e->ctx = new Context(buff_);
        Request req(this,e);
        req.run();
        LOG("set modify fd=%d event=%d",e->fd,E_READ)
        _loop->modify(e,E_WRITE);
        return realSize;


    };

    int UnixSocket::_write(Event *&e) {

        auto buf = e->ctx->buff.data();
        LOG("%s##########", buf)
        size_t cap = strlen(buf);
        size_t pos = 0;
        do {
            int ret = ::send(e->fd, buf + pos, cap - pos, 0);
            if (-1 == ret) {
                if ((errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == EINTR)) {
                    ret = 0;
                    continue;
                }
                break;
            }
            pos += ret;
        } while (pos < cap);
        _close(e);
        return pos;

    };

    void UnixSocket::_close(Event *&e) {
        int fd = e->fd;
        this->_loop->unregister_(e);
        delete e;
        ::close(fd);
    };

    void UnixSocket::_start() {

        auto e = new Event(_lfd, E_READ);
        this->_loop->register_(e);
        for (int i = 0; i < 10; i++) {


            auto items = this->_loop->poll();

            for (auto item:items) {
                switch (item->ev_flags) {
                    case E_READ:
                        if (item->fd == _lfd) {
                            accept(item->fd);
                        } else {
                            _read(item);
                        }
                        break;
                    case E_WRITE:
                        _write(item);
                        break;

                    default:
                        LOG_ERR("undefined evt:%d", item->ev_flags);
                        _close(e);
                }
            }
        }

    };

}

