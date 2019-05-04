#ifndef BASE_SOCKET_H
#define BASE_SOCKET_H

#include "angemon.h"
using angemon::event::Event;
namespace angemon {
namespace base {
class SocketImp {
public:
  virtual int _create() = 0;
  virtual int _read(Event *&e) = 0;
  virtual int _write(Event *&e) = 0;
  virtual void _close(Event *&e) = 0;
  virtual void _start() = 0;
};

} // namespace base
} // namespace angemon

#endif // BASE_SOCKET_H
