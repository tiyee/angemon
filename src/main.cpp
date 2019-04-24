
#include "angemon/server.h"
#include <iostream>
using angemon::Server;
int main() {

  Server s("127.0.0.1", 8001);
  s.run();
}
