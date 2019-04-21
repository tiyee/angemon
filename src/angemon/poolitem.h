#ifndef POOLITEM_H
#define POOLITEM_H
#include <sys/types.h>
#include"pool.h"
namespace angemon {
class PoolItem{
    static Pool<PoolItem,2> _pool;
public:
       void * operator new(size_t size) ;
       void operator delete(void* pdead);
};
};
#endif // POOLITEM_H
