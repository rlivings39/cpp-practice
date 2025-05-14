#include "pub_sub.hpp"
#include <chrono>
#include <condition_variable>
#include <gtest/gtest.h>
#include <thread>
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

  b.subscribe("First", &sub1);

  p.publish("First", std::make_unique<ry::Message>(12));

  b.processMessagesOnce();

  ASSERT_EQ(sub1.fMessages["First"].size(), 1);
  ASSERT_TRUE(sub1.fMessages["First"].contains(12));
}

TEST(PubSub, MultiMessageMultiSubs) {
  ry::Broker b;
  ry::Publisher p(b);

  LoggingSubscriber sub1;
  LoggingSubscriber sub2;
  LoggingSubscriber sub3;

  b.subscribe("First", &sub1);
  b.subscribe("First", &sub2);
  b.subscribe("Second", &sub3);

  p.publish("First", std::make_unique<ry::Message>(12));
  p.publish("First", std::make_unique<ry::Message>(1));
  p.publish("Second", std::make_unique<ry::Message>(42));

  b.processMessagesOnce();

  ASSERT_EQ(sub1.fMessages["First"].size(), 2);
  ASSERT_TRUE(sub1.fMessages["First"].contains(12));
  ASSERT_TRUE(sub1.fMessages["First"].contains(1));

  ASSERT_EQ(sub2.fMessages["First"].size(), 2);
  ASSERT_TRUE(sub2.fMessages["First"].contains(12));
  ASSERT_TRUE(sub2.fMessages["First"].contains(1));

  ASSERT_EQ(sub3.fMessages["Second"].size(), 1);
  ASSERT_TRUE(sub3.fMessages["Second"].contains(42));
}

TEST(PubSub, MultiMessageMultiSubsMultiThreads) {
  ry::Broker b;
  ry::Publisher p(b);

  LoggingSubscriber sub1;
  LoggingSubscriber sub2;
  LoggingSubscriber sub3;

  std::condition_variable finished_cond;
  std::thread t1{&ry::Broker::processMessages, &b, &finished_cond};

  b.subscribe("First", &sub1);
  b.subscribe("First", &sub2);
  b.subscribe("Second", &sub3);

  std::thread t2{[&p]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    p.publish("First", std::make_unique<ry::Message>(12));
    p.publish("First", std::make_unique<ry::Message>(1));
    p.publish("Second", std::make_unique<ry::Message>(42));
  }};

  std::mutex finished_m;
  std::unique_lock<std::mutex> l{finished_m};
  finished_cond.wait_for(l, std::chrono::milliseconds(1000),
                         [&sub1, &sub2, &sub3]() {
                           return sub1.fMessages["First"].size() == 2 &&
                                  sub2.fMessages["First"].size() == 2 &&
                                  sub3.fMessages["Second"].size() == 1;
                         });

  b.stopProcessing();
  t1.join();
  t2.join();

  ASSERT_EQ(sub1.fMessages["First"].size(), 2);
  ASSERT_TRUE(sub1.fMessages["First"].contains(12));
  ASSERT_TRUE(sub1.fMessages["First"].contains(1));

  ASSERT_EQ(sub2.fMessages["First"].size(), 2);
  ASSERT_TRUE(sub2.fMessages["First"].contains(12));
  ASSERT_TRUE(sub2.fMessages["First"].contains(1));

  ASSERT_EQ(sub3.fMessages["Second"].size(), 1);
  ASSERT_TRUE(sub3.fMessages["Second"].contains(42));
}
