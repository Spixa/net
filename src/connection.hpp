#pragma once
#include "net.h"

namespace net {
  template<typename T>
  class connection : public std::enable_shared_from_this<connection<T>> {
  public:
    connection()
    {

    }

    ~connection()
    {

    }
  public:
    bool connect_to_server();
    bool disconnect();
    bool is_connected() const;

  public: 
    bool send(const message<T> msg);
  protected:
    // Each connection has a unique socket
    asio::ip::tcp::socket socket_;

    // This context is shared with the whole instance
    asio::io_context& asio_context_;

    // This queue holds all messages to be sent to the remote side
    tsqueue<message<T>> message_out_queue_;

    // This queue holds all messages that have been received from the
    // remote side of this connection. 
    tsqueue<owned_message>& message_in_queue_;

  };
}