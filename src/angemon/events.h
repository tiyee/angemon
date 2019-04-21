#ifndef EVENTS_H
#define EVENTS_H
#include "connection.h"
namespace angemon {
enum Event {
  EVENT_ACCEPT = 2,
  EVENT_READ = 4,
  EVENT_WRITE = 8,
  EVENT_DEL = 16,
  EVENT_ERROR = 128
};
struct EvtItem {
  struct Conn conn;
  Event evt;
};
} // namespace angemon

#endif // EVENTS_H
