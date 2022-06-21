#include <asio/error_code.hpp>
#include <cstdlib>
#include <optional>
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

