#include "request.h"
namespace angemon {
Request::Request(BaseSocket * sock_,Event* ctx_):_socket(sock_),evt(ctx_)
{

}
void Request::run() {
    char buff[12] = "hello world";
    evt->ctx->buff = buff;
}
}

