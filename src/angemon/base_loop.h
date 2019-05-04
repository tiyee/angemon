#ifndef LOOP_H
#define LOOP_H
#include "angemon.h"

using angemon::event::Event;
using std::map;
using std::vector;

namespace angemon {
namespace base {
class LoopImp {
public:
  virtual void register_(Event *&ev) = 0;
  virtual void unregister_(Event *&ev) = 0;
  virtual void modify(Event *&evt, int ev) = 0;
  virtual vector<Event *> poll() = 0;
};
} // namespace base
} // namespace angemon

#endif // LOOP_H
