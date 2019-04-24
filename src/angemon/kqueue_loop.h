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
  bool _register(Conn conn, Event ev);

public:
  kqueue_loop(int lfd);
  void register_(Conn conn, Event ev);
  void unregister(Conn conn, Event ev);
  void modify(Conn conn, Event ev);
  std::vector<EvtItem> poll(int ms);
  void close();
};

} // namespace angemon

#endif // KQUEUE_LOOP_H
