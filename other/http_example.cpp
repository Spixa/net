#include <asio/error_code.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <chrono>
#include <thread>

#ifdef _WIN32
  #define _WIN32_WINNT 0x0A00
#endif

#include "asio.hpp"
#include "asio/ts/buffer.hpp"
#include "asio/ts/internet.hpp"

using asio::ip::tcp;
using namespace std::chrono_literals;

std::vector<char> buffer(20 * 1024);

void GrabSomeData(tcp::socket& socket) {
  socket.async_read_some(asio::buffer(buffer.data(), buffer.size()),
    [&](std::error_code ec, std::size_t length) {
      if (!ec) {
        std::cout << "\n\nRead: " << length << " bytes\n\n";

        for (int i = 0; i < length; i++) {
          std::cout << buffer[i];
        }

        GrabSomeData(socket);
      }
    }
  );
}

int main() 
{
  asio::error_code ec;
  asio::io_context ctx;

  // prime fake tasks for the context so it is working and doesn't exist immediately
  asio::io_context::work idleWork(ctx);

  // worker thread for the context (so it doesnt stop the program when it has to wait)
  std::thread thr_ctx = std::thread([&]() { ctx.run(); });

  // construct address
  asio::ip::tcp::endpoint endpoint(asio::ip::make_address("44.236.48.31", ec), 80);
  
  // socket
  asio::ip::tcp::socket socket(ctx);

  // instruct socket to connect to the endpoint
  socket.connect(endpoint, ec);

  if (!ec) {
    std::cout << "connected.\n";
  }
  else {
    std::cerr << "failed to connect to server: " << ec.message() << std::endl;
  }

  if (socket.is_open()) {
    GrabSomeData(socket);
    std::string request = 
      "GET /index.html HTTP/1.1\r\n"
      "Host: example.com\r\r\n"
      "Connection: close\r\n\r\n";
    
    socket.write_some(asio::buffer(request.data(), request.size()), ec);
    std::this_thread::sleep_for(2000ms);
  }
  thr_ctx.join();
}