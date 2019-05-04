#include "unix_socket.cpp"
#include "linux_loop.cpp"
#include "loop.h"
using namespace std;
using namespace angemon::base;
using angemon::UnixSocket;
using namespace angemon::linux;
int main() {
 angemon::base::Loop * loop_ = new  angemon::linux::Loop();
  UnixSocket sock("127.0.0.1", 8001, loop_);

  int lfd = sock._create();
  if (lfd < 0) {
    cout << "error" << endl;
    LOG("socket creat error")
    exit(2);
  }
  LOG("socket success fd=%d", lfd)

  sock._start();

  return 0;
}
