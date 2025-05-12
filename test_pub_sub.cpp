#include "pub_sub.hpp"
#include <gtest/gtest.h>

TEST(PubSub, SingleMessage) {
  ry::Broker b;
  ry::Publisher p(b);

  b.subscribe("First", std::make_unique<ry::PrintingSubscriber>(std::cerr));
  b.subscribe("First", std::make_unique<ry::PrintingSubscriber>(std::cerr));
  b.subscribe("Second", std::make_unique<ry::PrintingSubscriber>(std::cerr));

  p.publish("Second", std::make_unique<ry::Message>(1));
  p.publish("Second", std::make_unique<ry::Message>(2));
  p.publish("First", std::make_unique<ry::Message>(12));

  b.processMessages();
}
