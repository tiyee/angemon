//
// Created by tiyee on 2021/8/25.
//

#include "session.h"

angemon::Session::Session(angemon::Socket *sock) : _socket(sock) {

}
angemon::Session::~Session() {
    LOG("~Session");
}

void angemon::Session::Read() {

    for (;;) {
        uint32_t free_buf_len = _buff.GetWritableSize();
        if (free_buf_len < (kReadBufSize + 1))  // reserve 1 byte for text protocol to add '\0'
            _buff.Extend(kReadBufSize + 1);

        int n = _socket->Read(_buff.GetWritableBuffer(), kReadBufSize);
        if (n <= 0)
            break;
        _buff.IncrSize(n);
    }

    LOG("Conn::Read len=%d", _buff.GetSize())
    onWrite();
}

void angemon::Session::Write() {
    while (_buff.IsReadable()) {
        int ret = _socket->Write(_buff.GetReadableBuff(), std::min(int(_buff.GetReadableSize()), kMaxSendSize));
        if (ret <= 0) {
            break;
        }
        _buff.IncrPos(ret);
    }
    if (!_buff.IsReadable()) {
        onClose();
    }
}

void angemon::Session::Close() {
    _socket->onClose();
}

void angemon::Session::onRead() {
    _socket->GetEventLoop()->runInLoop([this]() { this->Read(); });
}

void angemon::Session::onWrite() {
    _socket->GetEventLoop()->runInLoop([this]() { this->Write(); });
}

void angemon::Session::onClose() {
    Close();
}


