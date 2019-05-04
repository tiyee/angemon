#ifndef CONTEXT_H
#define CONTEXT_H
#include "common.h"
using std::string;
namespace angemon {
struct Context {
  string buff;
  explicit Context(string buff_) { this->buff = std::move(buff_); }
};

} // namespace angemon
#endif // CONTEXT_H
