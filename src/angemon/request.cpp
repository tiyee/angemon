#include "request.h"
namespace angemon {
Request::Request(SocketImp *sock_, Event *ctx_) : _socket(sock_), evt(ctx_) {}
void Request::run() {
  char buf[1000] = {0};
  sprintf(buf, "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n%s",
          "Hello world!\n");
  evt->ctx->buff = buf;
}
} // namespace angemon
