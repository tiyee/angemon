#ifndef EPOLL_H
#define EPOLL_H
#include "loop_impl.h";
namespace angemon {
class Loop : public LoopImpl {
 public:
  Loop();
};

}  // namespace angemon

#endif  // EPOLL_H
