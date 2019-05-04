
/**
 * @link https://www.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2
 */
#ifndef UNIX_LOOP_H
#define UNIX_LOOP_H

#include "base_loop.h"

using angemon::base::LoopImp;
using angemon::event::Event;
using std::map;
using std::vector;
namespace angemon {

class Loop : public LoopImp {
private:
  map<int, Event *> _active;
  int _kfd;

public:
  Loop();

  ~Loop() = default;

  void register_(Event *&evt) final;

  void unregister_(Event *&evt) final;

  void modify(Event *&evt, int ev) final;

  vector<Event *> poll() final;
};

} // namespace angemon

#endif // UNIX_LOOP_H
