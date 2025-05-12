#include "pub_sub.hpp"
#include <algorithm>
#include <any>
#include <format>
#include <iostream>

/**
 * TODO
 * 1. Make testable
 * 2. Make multithreaded
 * 3. Revisit decisions to make things unique_ptr
 *
 */
namespace ry {
void PrintingSubscriber::receive(std::string aTopic, const Message &aMsg) {
  this->fOs << std::format("Topic {}; Data {}\n", aTopic,
                           std::any_cast<int>(aMsg.fData));
}

void Broker::publish(std::string aTopic, std::unique_ptr<Message> aMsg) {
  this->fMessages[aTopic].push(std::move(aMsg));
}

void Broker::subscribe(std::string aTopic, std::unique_ptr<ISubscriber> aSub) {
  this->fTopics.insert({aTopic, std::move(aSub)});
}

void Broker::processMessages() {
  for (auto &[topic, messageQueue] : this->fMessages) {
    if (messageQueue.empty()) {
      continue;
    }
    auto subRange = fTopics.equal_range(topic);
    while (!messageQueue.empty()) {
      auto &msg = messageQueue.front();
      std::for_each(subRange.first, subRange.second,
                    [this, &msg, &topic](auto &aSub) { aSub.second->receive(topic, *msg); });
      messageQueue.pop();
    }
  }
}

void Publisher::publish(std::string aTopic, std::unique_ptr<Message> aMsg) {
  this->fBroker.publish(aTopic, std::move(aMsg));
}
} // namespace ry
