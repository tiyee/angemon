#ifndef ACCEPTOR_H
#define ACCEPTOR_H
namespace angemon {
class Acceptor {
 public:
  Acceptor();
  int router();
  int bind();
  int listen();
};
}  // namespace angemon

#endif  // ACCEPTOR_H
