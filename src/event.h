#ifndef EVENT_H
#define EVENT_H
#include <functional>
namespace angemon {
const int E_READ = 0x1;
const int E_WRITE = 0x2;
const int E_SIGNAL = 0x4;
const int E_TIMEOUT = 0x8;
const int E_EDGE = 0x10; /* edge triggered */
const int E_ONCE = 0x20; /* one-time event */
const int E_IN_REACTOR = 0x40;
const int E_OUT_OF_TIMERHEAP = 0;
enum EventType { SOCK = 1, TIMER = 2, FC = 3 };

struct Event {
  EventType type;
  int evt_flag;
  int fd;
  void *data;
  std::function<int(Event *)> callback;
};
}  // namespace angemon

#endif  // EVENT_H
