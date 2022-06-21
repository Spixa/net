#include "lib.hpp"

namespace net {
  template<typename T>
  struct message_header
  {
    T id{};
    uint32_t size = 0;
  };

  template<typename T>
  struct message 
  {
    message_header<T> header{};
    std::vector<uint8_t> body;

    size_t size() const {
      return sizeof(message_header<T>) + body.size();
    }

    friend std::ostream& operator<<(std::ostream& os, const message<T>& msg) {
      os << "\n\t\tPrinting packet information\n\tID: " << int(msg.header.id) << "\n\tSize: " << msg.header.size << std::endl;
      return os;
    }

    // push any POD-like data and serialize them
    template<typename DataType>
    friend message<T>& operator<<(message<T>& msg, const DataType& data) {
      // Check that the type of the data being pushed is trivially copyable
      static_assert(std::is_standard_layout<DataType>::value,
      "Data is deemed too complex to be pushed as a valid message to the buffer");
      
      // Cache current size of the data being pushed to the message
      size_t i = msg.body.size();

      // Resize the body to sufficient size for the DataType to be pushed as well.
      msg.body.resize(msg.body.size() + sizeof(DataType));
      
      // Physically copy the data into the newly allocated vector space
      std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

      // Recalculate message size
      msg.header.size = msg.size();

      // Return the target message so it can be "chained"
      return msg;
    }

    template<typename DataType>
    friend message<T>& operator>>(message<T>& msg, DataType& data) {
      // Check that the type of the data being pushed is trivially copyable
      static_assert(std::is_standard_layout<DataType>::value,
      "Data is deemed too complex to be pushed as a valid message to the buffer");

      // Cache end of the vector
      size_t i = msg.body.size() - sizeof(DataType);
      
      // Physically copy the data from the vector into the user variable
      std::memcpy(&data, msg.body.data() + i, sizeof(DataType));
      
      // Shrink the vector to remove read bytes, and reset end position
      msg.body.resize(i);
      
      // Recalculate message size
      msg.header.size = msg.size();

      // Return the target message so it can be "chained"
      return msg;
    }
  };

  template<typename T>
  class connection;

  template<typename T>
  struct owned_message {
    std::shared_ptr<connection<T>> remote = nullptr;
    message<T> msg;

    friend std::ostream& operator<<(std::ostream& os, const owned_message<T>& msg) {
      os << msg.msg;
      return os;
    }
  };
}; // net