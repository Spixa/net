#pragma once
#include "net.h"

namespace net {
  template<typename T>
  class client_interface {
  public:
    client_interface() : socket_(context_)
    {

    }

    virtual ~client_interface() {
      disconnect();
    }

  public:
    bool connect(const std::string& host, const uint16_t port) {
      try
      {
        // Create connection
        connection_ = std::make_unique<connection<T>>(); // TODO

        // Create address
        asio::ip::tcp::resolver resolver(context_);
        endpoints_ = resolver.resolve(host, std::to_string(port));

        connection_->connect(resolver);

        context_thread_ = std::thread([this]() { context_.run(); });
      }
      catch(std::exception& e) {
        std::cerr << "Client Error: " << e.what() << std::endl;
      } 

      return false;
    }

    void disconnect()
    {
      if (is_connected()) {
        connection_->disconnect();
      }

      context_.stop();
      if(context_thread_.joinable()) {
        context_thread_.join();
      }

      connection_.release();
    }

    bool is_connected() const {
      if (connection_)
        return connection_->is_connected();
      else
        return false;
    }

    tsqueue<owned_message<T>>& incoming() {
      return message_in_;
    }

  protected:
    asio::io_context context_;
    std::thread context_thread_;
    asio::ip::tcp::socket socket_;
    std::unique_ptr<connection<T>> connection_;
  private:
    tsqueue<owned_message<T>> message_in_;
  };
};