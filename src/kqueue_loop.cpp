#include "kqueue_loop.h"

kqueue_loop::kqueue_loop(int lfd) : _lfd(lfd) { _efd = kqueue(); }
bool kqueue_loop::_register(struct Conn conn, Event evt) {
  struct kevent ev[2];
  int n = 0;
  if (evt == Event::EVENT_READ) {
    EV_SET(&ev[n++], conn.fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
  }
  if (evt == Event::EVENT_WRITE) {
    EV_SET(&ev[n++], conn.fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, nullptr);
  }

  int r = kevent(_efd, ev, n, NULL, 0, NULL);
  if (r < 0) {
    return false;
  }
  return true;
}
void kqueue_loop::register_(struct Conn conn, Event ev) { _register(conn, ev); }
void kqueue_loop::unregister(struct Conn conn, Event evt) {
  struct kevent ev[1];
  EV_SET(&ev[0], conn.fd, evt == EVENT_READ ? EVFILT_READ : EVFILT_WRITE,
         EV_DELETE, 0, 0, nullptr);
}
std::vector<struct EvtItem> kqueue_loop::poll(int ms) {
  struct timespec timeout;
  timeout.tv_sec = ms / 1000;
  timeout.tv_nsec = (ms % 1000) * 1000 * 1000;
  const int kMaxEvents = 20;
  struct kevent activeEvs[kMaxEvents];
  int n = kevent(_efd, NULL, 0, activeEvs, kMaxEvents, &timeout);
  std::vector<struct EvtItem> items;
  for (int i = 0; i < n; ++i) {
    int fd = activeEvs[i].ident;
    int events = activeEvs[i].filter;
    struct EvtItem item;
    if (events == EVFILT_READ) {
      if (fd == _lfd) {
        item = {Conn(fd), EVENT_ACCEPT};
      } else {
        item = {Conn(fd), EVENT_READ};
      }
    } else if (events == EVFILT_WRITE) {
      item = {Conn(fd), EVENT_WRITE};
    } else {
      printf("unknown event");
    }
    items.push_back(item);
  }
  return items;
}
