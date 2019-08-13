#include "kqueue.h"
namespace angemon {
void Loop::add(Event*& ev) { _map[ev->fd] = ev; };
void Loop::remove(Event*& ev) { _map.erase(ev->fd); };
void Loop::modify(Event*& oev, Event*& nev) {
  remove(oev);
  add(nev);
}
}  // namespace angemon
