//
// Created by tiyee on 2021/8/25.
//

#include "socket.h"

#include <utility>
#include "global.h"

extern angemon::ThreadObject<angemon::EventLoop, 10> gThreadObject;

angemon::Socket::Socket(int fd, bool isLocal, std::string ip, const uint16_t port)
        : _fd(fd), _isLocal(isLocal), _ip(std::move(ip)), _port(port) {
    _session = nullptr;
    _eventLoop = nullptr;
    _state = -1;
}

angemon::Socket::~Socket() {
    LOG("~Socket")
}

void angemon::Socket::onRead() {
    LOG("Socket::onRead");
    if (_isLocal && _state == SOCKET_STATE_LISTENING) {
        _eventLoop->runInLoop([this]() { this->Accept(); });
    } else {
        u_long avail = 0;
        if ((ioctl(_fd, FIONREAD, &avail) == SOCKET_ERROR) || (avail == 0)) {
            onClose();
        } else {
            _session->onRead();
            // process receive data and FIN packet simultaneously, recv() return 0 means peer close the socket
            if (_state == SOCKET_STATE_PEER_CLOSING) {
                onClose();
            }
        }
    }

}

void angemon::Socket::Accept() const {
    int fd = 0;
    sockaddr_in peer_addr{};
    socklen_t addr_len = sizeof(sockaddr_in);
    char ip_str[64];

    while (true) {
        fd = accept(_fd, (sockaddr *) &peer_addr, &addr_len);
        if (fd == INVALID_SOCKET) {
            if (errno != EWOULDBLOCK) {
                LOG_ERR("accept errno=%d\n", errno);
            }

            break;
        }


        uint32_t ip = ntohl(peer_addr.sin_addr.s_addr);
        uint16_t port_ = ntohs(peer_addr.sin_port);

        snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d", ip >> 24, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);

        LOG("accept fd=%d address=%s:%d", fd, ip_str, port_);
        std::string ipStr = ip_str;
        auto *pSocket = new Socket(fd, false, ipStr, port_);
        auto el = gThreadObject.GetObject(fd);// random thread
        pSocket->SetEventLoop(el);
        SetNoDelay(fd);
        SetNonblock(fd, true);
        auto session = new Session(pSocket);
        pSocket->SetSession(session);
        pSocket->SetState(SOCKET_STATE_CONNECTED);
        el->AddSocketEvent(fd, SOCKET_READ | SOCKET_EXCEP |

                               SOCKET_ADD_CONN, pSocket);
    }
}

void angemon::Socket::onWrite() {
#ifdef __APPLE__
    _eventLoop->RemoveSocketEvent(_fd, SOCKET_WRITE, this);
#endif
    if (_state == SOCKET_STATE_CONNECTED) {
        _session->onWrite();
    }
}

void angemon::Socket::onClose() {
    if (_state == SOCKET_STATE_CLOSING) {
        return;
    }
    _state = SOCKET_STATE_CLOSING;
    _eventLoop->runInLoop([this]() { this->Close(); });
}

void angemon::Socket::Close() {
    _state = SOCKET_STATE_CLOSING;
    _eventLoop->RemoveSocketEvent(_fd, SOCKET_ALL | SOCKET_DEL_CONN, this);
    if (!_isLocal) {
        delete _session;
    }
    close(_fd);
    _eventLoop->runInLoop([this]() { delete this; });
}

void angemon::Socket::onError() {
    onClose();

}

void angemon::Socket::onConnect() {


}

void angemon::Socket::Connect(std::string server_ip, uint16_t port) {
    printf("BaseSocket::Connect, server_ip=%s, port=%d\n", server_ip.c_str(), port);


    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == INVALID_SOCKET) {
        printf("socket failed, err_code=%d\n", errno);
        return;
    }

    SetNonblock(fd, true);
    SetNoDelay(fd);

    sockaddr_in serv_addr{};
    SetAddr(server_ip, port, &serv_addr);
    int ret = connect(fd, (sockaddr *) &serv_addr, sizeof(serv_addr));
    if ((ret == SOCKET_ERROR) && (!IsBlock(errno))) {
        printf("connect failed, err_code=%d\n", errno);
        close(fd);
        return;
    }
    if(ret<0&&errno == EINPROGRESS) {

    }

    GetBindAddr(fd, server_ip, port);
    auto socket = new Socket(fd, false, server_ip, port);
    socket->SetState(SOCKET_STATE_CONNECTING);
    auto session = new Session(socket);
    socket->SetSession(session);
    auto el = gThreadObject.GetObject(fd);
    el->AddSocketEvent(fd, SOCKET_ALL | SOCKET_ADD_CONN, this);
}

int angemon::Socket::Read(void *buf, size_t len) {
    int n = (int) recv(_fd, (char *) buf, len, 0);
    if (n == 0) {
        _state = SOCKET_STATE_PEER_CLOSING;
    }

    return n;
}

int angemon::Socket::Write(void *buf, size_t len) {
    LOG("Socket::Write %s %d", buf, len);
    int ret = (int) send(_fd, (char *) buf, len, 0);
    if (ret == SOCKET_ERROR) {
        if (IsBlock(errno)) {
#ifdef __APPLE__
            _eventLoop->AddSocketEvent(_fd, SOCKET_WRITE, this);
#endif
            ret = 0;
        } else {
            LOG_ERR("send failed, error code: %d ,err=%s", errno, strerror(errno));
        }
    }

    return ret;
}

void angemon::Socket::StartListen() {
    std::string ip = "127.0.0.1";
    uint16_t port = 37188;
    int fd = angemon::Listen(ip, port);
    assert(fd);
    auto el = gThreadObject.GetMainObject();
    GetBindAddr(fd, ip, port);
    auto *socket = new Socket(fd, true, ip, port);
    socket->SetEventLoop(el);
    socket->SetState(SOCKET_STATE_LISTENING);
    el->SetThreadId(std::this_thread::get_id());
    LOG("Listen on %s:%d fd=%d tid=%d \n", socket->GetIp().c_str(), port, fd, std::this_thread::get_id());
    el->AddSocketEvent(fd, SOCKET_READ | SOCKET_EXCEP | SOCKET_ADD_CONN, socket);
    el->Start();

}
