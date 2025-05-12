#include "pub_sub.hpp"
#include <gtest/gtest.h>

TEST(PubSub, SingleMessage) {
  ry::Subscriber s1, s2;
  ry::Broker b;
  ry::Publisher p(b);

  b.subscribe("First", std::make_unique<ry::Subscriber>());
  b.subscribe("First", std::make_unique<ry::Subscriber>());
  b.subscribe("Second", std::make_unique<ry::Subscriber>());

  p.publish("Second", std::make_unique<ry::Message>(1));
  p.publish("Second", std::make_unique<ry::Message>(2));
  p.publish("First", std::make_unique<ry::Message>(12));

  b.processMessages();
}
