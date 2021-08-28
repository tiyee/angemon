//
// Created by tiyee on 2021/8/25.
//

#ifndef ANGEMON_SESSION_H
#define ANGEMON_SESSION_H


#include "socket_utils.h"
#include "bytes_buffer.h"

namespace angemon {
    class Socket;

    const int kHeartBeartInterval = 3000;
    const int kConnTimeout = 16000;

    const int kMaxSendSize = 128 * 1024;
    const int kReadBufSize = 2048;

    class Session {
    public:
        Session(Socket *sock);

        ~Session();

        void onRead();

        void onWrite();

        void onConfirm();

        void onClose();

        void onTimer();

        void Confirm();

        void Read();

        void Write();

        void Close();

        void Timer();

    private:
        Socket *_socket;
        BytesBuffer _buff;


    };
}


#endif //ANGEMON_SESSION_H
