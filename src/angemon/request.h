#ifndef REQUEST_H
#define REQUEST_H
#include "base_socket.h"
#include "event.h"
using angemon::base::SocketImp;
using angemon::event::Event;
namespace angemon {
class Request {
private:
  SocketImp *_socket;
  Event *evt;

public:
  Request(SocketImp *sock_, Event *ctx_);
  void run();
};
} // namespace angemon

#endif // REQUEST_H
