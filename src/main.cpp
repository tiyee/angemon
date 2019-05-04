#include "socket.cpp"
#ifdef OS_MACOSX
#include "unix_loop.cpp"
#endif
#ifdef OS_LINUX
#include "linux_loop.cpp"
#endif


#include "base_loop.h"
using namespace std;
using  angemon::base::LoopImp;
using angemon::Socket;
using  angemon::Loop;
int main() {
LoopImp * loop_ = new  Loop();
  Socket sock("127.0.0.1", 8001, loop_);
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
