#include "server.h"
#include "kqueue_loop.h"
namespace angemon {
Server::Server(const char *ip, int port) {
  _addr.sin_family = AF_INET;
  _addr.sin_port = htons(port);
  _addr.sin_addr.s_addr = inet_addr(ip);

  _lfd = socket(AF_INET, SOCK_STREAM, 0);
  int r = ::bind(_lfd, (struct sockaddr *)&_addr, sizeof(struct sockaddr));
  if (r < 0) {
    printf("bind error ");
  }
  r = listen(_lfd, 20);
  if (r < 0) {
    printf("lst error ");
  }
  setNonBlock(_lfd);

  _loop = new kqueue_loop(_lfd);

  _loop->register_(Conn(_lfd), Event::EVENT_READ);
  while (true) {

    auto l = _loop->poll(3000);
    for (int i = 0; i < l.size(); ++i) {
      if (l[i].conn.fd == _lfd) {
        int cfd = accept_(l[i].conn.fd);
        setNonBlock(cfd);
        _loop->register_(Conn(cfd), Event::EVENT_READ);
      } else {
        if (l[i].evt == Event::EVENT_READ) {
          auto n = read_(l[i].conn);
          printf("%d\n", n);
          _loop->unregister(l[i].conn, Event::EVENT_READ);
          close(l[i].conn.fd);
        }
      }
    }
  }
}
int Server::accept_(int fd) {
  struct sockaddr_in raddr;
  socklen_t rsz = sizeof(raddr);
  int cfd = accept(fd, (struct sockaddr *)&raddr, &rsz);
  return cfd;
}
int Server::read_(Conn conn) {
  char buf[4096];
  auto n = ::read(conn.fd, buf, sizeof buf);
  int r = ::write(conn.fd, buf, n);
  printf("read %d bytes\n", r);
  return n;
}
void Server::setNonBlock(int fd) const {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
    printf("fcntl failed");
  }
  int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  if (r < 0) {
    printf("fcntl failed");
  }
}
void Server::run() {
  while (true) {

    auto l = _loop->poll(3000);
    for (int i = 0; i < l.size(); ++i) {
      if (l[i].conn.fd == _lfd) {
        int cfd = accept_(l[i].conn.fd);
        setNonBlock(cfd);
        _loop->register_(Conn(cfd), Event::EVENT_READ);
      } else {
        if (l[i].evt == Event::EVENT_READ) {
          auto n = read_(l[i].conn);
          printf("%d\n", n);
          _loop->unregister(l[i].conn, Event::EVENT_READ);
          close(l[i].conn.fd);
        }
      }
    }
  }
}

} // namespace angemon
