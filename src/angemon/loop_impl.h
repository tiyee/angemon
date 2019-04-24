#ifndef LOOP_IMPL_H
#define LOOP_IMPL_H
#include "connection.h"
#include "events.h"
#include <map>
#include <stddef.h>
#include <vector>
namespace angemon {
class LoopImpl {

public:
  virtual void close() = 0;
  virtual void register_(Conn conn, Event ev) = 0;
  virtual void unregister(Conn conn, Event ev) = 0;
  virtual void modify(Conn conn, Event ev) = 0;
  virtual std::vector<EvtItem> poll(size_t timeout) = 0;
  virtual Conn active_(int fd) = 0;
};
} // namespace angemon

#endif // LOOP_IMPL_H
