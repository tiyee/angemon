#ifndef LOOP_IMPL_H
#define LOOP_IMPL_H
#include "event.h"
namespace angemon {
class LoopImpl {
 public:
  LoopImpl();
  virtual void add(Event*& ev) = 0;
  virtual void remove(Event*& ev) = 0;
  virtual void modify(Event*& oev, Event*& nev) = 0;
  virtual const Event*& find(const int fd) = 0;
  virtual void poll(size_t ms) = 0;
};

}  // namespace angemon

#endif  // LOOP_IMPL_H
