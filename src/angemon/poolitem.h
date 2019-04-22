#ifndef POOLITEM_H
#define POOLITEM_H
#include "pool.h"
#include <sys/types.h>
namespace angemon {
class PoolItem {
  static Pool<PoolItem, 2> _pool;

public:
  void *operator new(size_t size);
  void operator delete(void *pdead);
};
};     // namespace angemon
#endif // POOLITEM_H
