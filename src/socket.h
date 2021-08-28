//
// Created by tiyee on 2021/8/25.
//

#ifndef ANGEMON_SOCKET_H
#define ANGEMON_SOCKET_H

#include "socket_utils.h"


namespace angemon {
    class EventLoop;

    class Session;

    class Socket {
    public :
        Socket(int fd, bool isLocal, std::string  ip, const uint16_t port);

        ~Socket();

        int GetHandle() const {
            return _fd;
        };

        void SetEventLoop(EventLoop *&el) {
            _eventLoop = el;
        };

        void SetState(int state) {
            _state = state;
        }

        EventLoop *GetEventLoop() const {
            return _eventLoop;
        };

        void SetSession(Session *&session) {
            _session = session;
        }


        std::string GetIp() const {
            return _ip;
        };


        uint16_t GetPort() const {
            return _port;
        }

        void onRead();

        void onWrite();

        void onClose();

        void onError();

        void onConnect();

        void Close();

        void Connect(std::string  server_ip, const uint16_t port);

        int Read(void *buf, size_t len);

        int Write(void *buf, size_t len);
        static void StartListen();

    private:
        void Accept() const;

    private:
        const int _fd;
        const bool _isLocal;
        EventLoop *_eventLoop;
        Session *_session;
        const std::string _ip;
        const uint16_t _port;
        int _state;

    };
}


#endif //ANGEMON_SOCKET_H
