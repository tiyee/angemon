#ifndef KQUEUE_H
#define KQUEUE_H
#include "loop_impl.h"
#include "unordered_map"
namespace angemon {
class Loop : public LoopImpl {
 private:
  const int _kfd;
  std::unordered_map<int, Event*&> _map;

 public:
  explicit Loop(int fd) : _kfd(fd){};
  void add(Event*& ev) override;
  void remove(Event*& ev) override;
  void modify(Event*& oev, Event*& nev) override;
  const Event*& find(const int fd) override;
  void poll(size_t ms) override;
};
}  // namespace angemon

#endif  // KQUEUE_H
