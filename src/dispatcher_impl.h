#ifndef DISPATCHER_IMPL_H
#define DISPATCHER_IMPL_H

class DispatcherImpl {
 public:
  virtual void router(int evt_flag) = 0;
};

#endif  // DISPATCHER_IMPL_H
