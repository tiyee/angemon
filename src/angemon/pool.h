#ifndef ANGEMON_POOL_H
#define ANGEMON_POOL_H

#include "common.h"
using std::list;
namespace angemon {
template <class T> class Pool {

private:
  static list<T *> _items;

public:
  const size_t MAX_ITEMS = 1024;

  inline Pool() = default;

  inline ~Pool() = default;

  inline size_t put(T *t);

  inline T *get();
};

template <class T> std::list<T *> Pool<T>::_items;

template <class T> T *Pool<T>::get() {

  if (!_items.empty()) {

    auto it = _items.begin();
    T *p = *it;
    _items.pop_front();
    return p;
  } else {
    return (T *)malloc(sizeof(T));
    ;
  }
}

template <class T> size_t Pool<T>::put(T *t) {

  _items.push_back(t);
  return _items.size();
}

} // namespace angemon

#endif // POOL_H
