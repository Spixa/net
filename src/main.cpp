#include "client.hpp"
#include "net.h"

enum class CustomMessageTypes : uint32_t 
{
  FireBullet,
  MovePlayer,
};

class CustomClient : public net::client_interface<CustomMessageTypes> {
public:
  bool FireBullet(float x, float y) {
    net::message<CustomMessageTypes> msg;
    msg.header.id = CustomMessageTypes::FireBullet;
    msg << x << y;
    return send(msg);
  }
};

int main()
{
  CustomClient c;
  c.connect("localhost", 37549);
  c.FireBullet(2.0f, 5.0f);

  return 0;
}