#ifndef LOOP_IMPL_H
#define LOOP_IMPL_H
#include "connection.h"
#include "events.h"
#include <stddef.h>
#include <vector>
namespace angemon {
class LoopImpl {

public:
  virtual void close() = 0;
  virtual void register_(struct Conn conn, Event ev) = 0;
  virtual void unregister(struct Conn conn, Event ev) = 0;
  virtual void modify(struct Conn conn, Event ev) = 0;
  virtual std::vector<struct EvtItem> poll(size_t timeout) = 0;
};
} // namespace angemon

#endif // LOOP_IMPL_H
