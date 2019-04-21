#ifndef KQUEUE_LOOP_H
#define KQUEUE_LOOP_H

#include "connection.h"
#include "events.h"
#include "loop_impl.h"
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <stddef.h>
#include <string>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
namespace angemon {
class kqueue_loop : public LoopImpl {
private:
  int _efd;
  int _lfd;
  bool _register(struct Conn conn, Event ev);

public:
  kqueue_loop(int lfd);
  void register_(struct Conn conn, Event ev);
  void unregister(struct Conn conn, Event ev);
  void modify(struct Conn conn, Event ev);
  std::vector<struct EvtItem> poll(int ms);
};

} // namespace angemon

#endif // KQUEUE_LOOP_H
