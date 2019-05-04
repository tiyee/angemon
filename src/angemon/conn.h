#ifndef ANGEMON_CONN_H
#define ANGEMON_CONN_H

#include "common.h"
#include "ctime"
#include "pool.h"
namespace angemon {
namespace pools {
using namespace std;
using std::string;
using std::vector;

class Conn {
private:
  static Pool<Conn> _pool;
  string _buff;
  size_t _buff_read;
  size_t _buff_write;
  size_t _buff_cap;
  int _fd;
  const time_t _ctime;
  time_t _mtime;
  void Mtime(time_t t_);

public:
  inline Conn();



  Conn(const Conn &conn) = delete;

  Conn &operator=(const Conn &conn) = delete;

  int _evt;

  inline int FD();

  inline void FD(int fd_);

  inline size_t BuffRead(); // index of buff read

  inline void BuffRead(size_t);

  inline size_t BuffWrite(); // index of buff wrote

  inline void BuffWrite(size_t);

  inline size_t BuffCap(); // buff real size

  inline void BuffCap(size_t);

  inline string Buff();

  inline void Buff(string buff_);
  inline time_t Ctime();
  inline time_t Mtime();

  static void *operator new(size_t t);

  static void operator delete(void *ptr);
};

Pool<Conn> Conn::_pool;

Conn::Conn()
    : _buff(""), _buff_read(0), _buff_write(0),_buff_cap(0), _fd(0), _evt(0),
      _ctime(time(nullptr)), _mtime(time(nullptr)) {}



void *Conn::operator new(size_t t) {

  return Conn::_pool.get();
}

void Conn::operator delete(void *ptr) {


  Conn::_pool.put((Conn *)ptr);
}

inline int Conn::FD() { return this->_fd; };

inline void Conn::FD(int fd_) {
  this->_fd = fd_;
  this->Mtime(time(nullptr));
};

inline size_t Conn::BuffRead() { return this->_buff_read; };

inline void Conn::BuffRead(size_t t_) {
  this->_buff_read = t_;
  this->Mtime(time(0));
};

inline size_t Conn::BuffWrite() { return this->_buff_write; };

inline void Conn::BuffWrite(size_t t_) {
  this->_buff_write = t_;
  this->Mtime(time(nullptr));
};

inline size_t Conn::BuffCap() { return this->_buff_cap; };

inline void Conn::BuffCap(size_t t_) { this->_buff_cap = t_; };

inline string Conn::Buff() { return this->_buff; };

inline void Conn::Buff(string buff_) {
  this->_buff = std::move(buff_);
}
inline time_t Conn::Ctime() { return this->_ctime; }
inline time_t Conn::Mtime() { return this->_mtime; }
inline void Conn::Mtime(time_t t_) { this->_mtime = t_; }
} // namespace pools
} // namespace angemon

#endif // CONN_H
