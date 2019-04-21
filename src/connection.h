#ifndef CONNECTION_H
#define CONNECTION_H
#include <stddef.h>
#include <string>
struct Conn {
  int fd;
  std::string readed;
  size_t written;
  bool writeEnabled;
  Conn(int fd_ = 0) : fd(fd_), written(0), writeEnabled(false) {}
};
#endif // CONNECTION_H
