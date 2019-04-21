#include "pool.h"
namespace angemon {
template <class T, int N> Pool<T, N>::Pool() {
  _length = N;
  for (int i = 0; i < _length; ++i) {
    _items.push_back(new T());
  }
}
template <class T, int N> int Pool<T, N>::put(T const &t) {
  _items.push_back(t);
  return _items.size();
}
template <class T, int N> T *Pool<T, N>::get() {
  if (!_items.empty()) {
    auto it = _items.begin();
    T *p = *it;
    _items.pop_front();
    return p;
  } else {
    T *p = new T;
    _length++;
    return p;
  }
}
template <class T, int N> int Pool<T, N>::size() { return _length; }
} // namespace angemon
