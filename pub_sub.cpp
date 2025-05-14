#include "pub_sub.hpp"
#include <algorithm>
#include <any>
#include <condition_variable>
#include <format>
#include <mutex>

/**
 * TODO
 * 1. Revisit decisions to make things unique_ptr
 *
 */
namespace ry {
void PrintingSubscriber::receive(std::string aTopic, const Message &aMsg) {
  this->fOs << std::format("Topic {}; Data {}\n", aTopic,
                           std::any_cast<int>(aMsg.fData));
}

void Broker::publish(std::string aTopic, std::unique_ptr<Message> aMsg) {
  std::lock_guard<std::mutex> l{this->fQueueLock};
  this->fMessages[aTopic].push(std::move(aMsg));
  this->fMessagesCond.notify_all();
}

void Broker::subscribe(std::string aTopic, ISubscriber *aSub) {
  std::lock_guard<std::mutex> l{this->fQueueLock};
  this->fTopics.insert({aTopic, aSub});
}

void Broker::stopProcessing() {
  std::lock_guard<std::mutex> l{this->fQueueLock};
  this->fStopProcessing = true;
  this->fMessagesCond.notify_all();
}

void Broker::processMessages(std::condition_variable *aProcessed) {
  std::unique_lock<std::mutex> l(this->fQueueLock);
  while (true) {
    if (this->fStopProcessing) {
      return;
    }
    this->fMessagesCond.wait(l);
    processMessagesImpl();
    if (aProcessed) {
      aProcessed->notify_all();
    }
  }
}

void Broker::processMessagesOnce() {
  processMessagesImpl();
}

void Broker::processMessagesImpl() {
  for (auto &[topic, messageQueue] : this->fMessages) {
    if (messageQueue.empty()) {
      continue;
    }
    auto subRange = fTopics.equal_range(topic);
    while (!messageQueue.empty()) {
      auto &msg = messageQueue.front();
      std::for_each(subRange.first, subRange.second,
                    [this, &msg, &topic](auto &aSub) {
                      aSub.second->receive(topic, *msg);
                    });
      messageQueue.pop();
    }
  }
}

void Publisher::publish(std::string aTopic, std::unique_ptr<Message> aMsg) {
  this->fBroker.publish(aTopic, std::move(aMsg));
}
} // namespace ry
