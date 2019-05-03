#ifndef REQUEST_H
#define REQUEST_H
#include"base_socket.h"
#include "event.h"
using angemon::event::Event;
using angemon::base::BaseSocket;
namespace angemon {
class Request
{
private:
    BaseSocket* _socket;
    Event* evt;
public:
    Request(BaseSocket* sock_,Event* ctx_);
    void run();
};
}


#endif // REQUEST_H
