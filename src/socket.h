#ifndef SOCKET_H
#define SOCKET_H
#include "event.h"
namespace angemon {
class Socket {
 public:
  Socket();
  static void accept(int fd);
  static void write(const Event*& ev);
  static void read(const Event*& ev);
  static void close(const Event*& ev);
};

}  // namespace angemon

#endif  // SOCKET_H
