#include "pub_sub.hpp"
#include <gtest/gtest.h>
#include <unordered_set>

struct LoggingSubscriber : public ry::ISubscriber {
  void receive(std::string aTopic, const ry::Message &aMsg) override {
    fMessages[aTopic].insert(std::any_cast<int>(aMsg.fData));
  }
  std::unordered_map<std::string, std::unordered_multiset<int>> fMessages;
};

TEST(PubSub, SingleMessage) {
  ry::Broker b;
  ry::Publisher p(b);

  LoggingSubscriber sub1;

  b.subscribe("First",  &sub1);

  p.publish("First", std::make_unique<ry::Message>(12));

  b.processMessages();

  ASSERT_EQ(sub1.fMessages["First"].size(), 1);
  ASSERT_TRUE(sub1.fMessages["First"].contains(12));
}

TEST(PubSub, MultiMessageMultiSubs) {
  ry::Broker b;
  ry::Publisher p(b);

  LoggingSubscriber sub1;
  LoggingSubscriber sub2;
  LoggingSubscriber sub3;

  b.subscribe("First",  &sub1);
  b.subscribe("First",  &sub2);
  b.subscribe("Second", &sub3);

  p.publish("First", std::make_unique<ry::Message>(12));
  p.publish("First", std::make_unique<ry::Message>(1));
  p.publish("Second", std::make_unique<ry::Message>(42));

  b.processMessages();

  ASSERT_EQ(sub1.fMessages["First"].size(), 2);
  ASSERT_TRUE(sub1.fMessages["First"].contains(12));
  ASSERT_TRUE(sub1.fMessages["First"].contains(1));

  ASSERT_EQ(sub2.fMessages["First"].size(), 2);
  ASSERT_TRUE(sub2.fMessages["First"].contains(12));
  ASSERT_TRUE(sub2.fMessages["First"].contains(1));

  ASSERT_EQ(sub3.fMessages["Second"].size(), 1);
  ASSERT_TRUE(sub3.fMessages["Second"].contains(42));
}
