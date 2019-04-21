#ifndef POOL_H
#define POOL_H
#include <iostream>
#include <list>
namespace angemon {
using std::list;
template <class T, int N> class Pool {
private:
  list<T *> _items;
  int _length;

public:
  Pool();
  int put(T const &t);
  T *get();
  int size();
};

} // namespace angemon

#endif // POOL_H
