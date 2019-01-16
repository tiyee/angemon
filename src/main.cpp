

#include <boost/asio.hpp>
#include <iostream>
using namespace boost::asio;
class Server {
  typedef ip::tcp::socket socket_type;
  typedef std::shared_ptr<socket_type> sock_ptr;

 public:
  Server() : m_acceptor(m_io, ip::tcp::endpoint(ip::tcp::v4(), 6688)) {
    accept();
  }
  void run() { m_io.run(); }

 private:
  void accept() {
    sock_ptr sock(new socket_type(m_io));
    m_acceptor.async_accept(*sock, std::bind(&Server::accept_handler, this,
                                             std::placeholders::_1, sock));
  }
  void accept_handler(const boost::system::error_code& ec, sock_ptr sock) {
    if (ec) {
      return;
    }
    std::cout << "client:";
    std::cout << sock->remote_endpoint().address() << std::endl;
    sock->async_write_some(
        buffer("hello asio"),
        std::bind(&Server::write_handler, this, std::placeholders::_1));
    accept();
  }
  void write_handler(const boost::system::error_code&) {
    std::cout << "send msg complete." << std::endl;
  }

 private:
  io_service m_io;
  ip::tcp::acceptor m_acceptor;
};
int main() {
  try {
    std::cout << "server" << std::endl;
    Server svr;
    svr.run();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
