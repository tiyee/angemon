
/**
 * @link http://www.man7.org/linux/man-pages/man7/epoll.7.html
 */
#include "unix_loop.h"
#include <sys/event.h>
using angemon::event::E_READ;
using angemon::event::E_WRITE;
namespace angemon {

Loop::Loop()
    : _kfd(kqueue()){

      };
void Loop::register_(Event *&evt) {

  struct kevent evs[1];
  int n = 0;
  if (evt->ev_flags == E_READ) {
    EV_SET(&evs[n++], evt->fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
  }
  if (evt->ev_flags == E_WRITE) {
    EV_SET(&evs[n++], evt->fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, nullptr);
  }

  int r = kevent(_kfd, evs, n, nullptr, 0, nullptr);
  if (r < 0) {
    std::cout << "kevent error" << std::endl;
    return;
  }

  LOG("kevent success:: kid=%d", r)
  _active[evt->fd] = evt;
};

void Loop::unregister_(Event *&evt) {
  struct kevent ev[1];
  EV_SET(&ev[0], evt->fd, evt->ev_flags == E_READ ? EVFILT_READ : EVFILT_WRITE,
         EV_DELETE, 0, 0, nullptr);
  int r = kevent(_kfd, ev, 1, nullptr, 0, nullptr);
  if (r < 0) {
    std::cout << "kevent error" << std::endl;
    return;
  }
  int fd_ = evt->fd;
  auto iter = _active.find(fd_);
  _active.erase(iter);
  // delete evt;
};

void Loop::modify(Event *&evt, int ev) {
  struct kevent kev[2];
  EV_SET(&kev[0], evt->fd, evt->ev_flags == E_READ ? EVFILT_READ : EVFILT_WRITE,
         EV_DELETE, 0, 0, nullptr);
  EV_SET(&kev[1], evt->fd, ev == E_READ ? EVFILT_READ : EVFILT_WRITE, EV_ADD, 0,
         0, nullptr);
  int r = kevent(_kfd, kev, 2, nullptr, 0, nullptr);
  if (r < 0) {
    std::cout << "kevent error" << std::endl;
    return;
  }
  LOG("kevent success:: kid=%d", r)
  evt->ev_flags = ev;
};

vector<Event *> Loop::poll() {
  timespec timeout = {};
  timeout.tv_sec = 3000 / 1000;
  timeout.tv_nsec = (3000 % 1000) * 1000 * 1000;
  const int kMaxints = 20;
  struct kevent activeEvs[kMaxints];
  int n = kevent(_kfd, nullptr, 0, activeEvs, kMaxints, &timeout);
  LOG("poll return n=%d", n)
  std::vector<Event *> items;
  for (int i = 0; i < n; ++i) {
    int fd = activeEvs[i].ident;
    int events = activeEvs[i].filter;
    LOG("fd=%d filter=%d", fd, events)
    Event *item = nullptr;
    if (events == EVFILT_READ) {
      item = _active[fd];
      item->ev_flags = E_READ;

    } else if (events == EVFILT_WRITE) {
      item = _active[fd];
      LOG("poll fd=%d", item->fd);
      item->ev_flags = E_WRITE;

    } else {
      LOG_ERR("undefined evt:%d", events)
      continue;
    }
    items.push_back(item);
  }
  return items;
};

} // namespace angemon
