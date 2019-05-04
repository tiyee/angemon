#ifndef REQUEST_H
#define REQUEST_H
#include"base_socket.h"
#include "event.h"
using angemon::event::Event;
using angemon::base::SocketImp;
namespace angemon {
class Request
{
private:
    SocketImp* _socket;
    Event* evt;
public:
    Request(SocketImp* sock_,Event* ctx_);
    void run();
};
}


#endif // REQUEST_H
