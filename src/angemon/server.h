#ifndef SERVER_H
#define SERVER_H
#include "connection.h"
#include "loop_impl.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
using std::string;
namespace angemon {
class Server {
private:
  int _lfd;
  struct sockaddr_in _addr;
  LoopImpl *_loop;

public:
  Server(const Server &) = delete;
  Server &operator=(const Server &) = delete;
  Server(const char *ip, int port);
  int bind();
  void setNonBlock(int fd) const;
  int accept_(int fd);
  int read_(Conn conn);
  void run();
};

} // namespace angemon

#endif // SERVER_H
