#include "unix_socket.cpp"
#include "unix_loop.cpp"
using namespace std;
using angemon::base::Loop;
using angemon::UnixSocket;
using angemon::unix::UnixLoop;
int main() {
 Loop * loop_ = new  UnixLoop();
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
