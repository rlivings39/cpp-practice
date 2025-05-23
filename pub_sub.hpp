#pragma once

#include <any>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_map>

namespace ry {
struct Message {
  Message(std::any aData) : fData(aData) {}

  std::any fData;
};

struct ISubscriber {
  virtual void receive(std::string aTopic, const Message &aMsg) = 0;
};

struct PrintingSubscriber : public ISubscriber {
  PrintingSubscriber(std::ostream &aOs = std::cout) : fOs(aOs) {}
  void receive(std::string aTopic, const Message &aMsg) override;

private:
  std::ostream &fOs;
};

// TODO why have this?
struct Broker {
  void publish(std::string aTopic, std::unique_ptr<Message> aMsg);
  void subscribe(std::string aTopic, ISubscriber *aSub);
  void processMessages(std::condition_variable *aProcessed);
  void processMessagesOnce();
  void stopProcessing();

private:
  void processMessagesImpl();
  std::unordered_multimap<std::string, ISubscriber *> fTopics;
  std::unordered_map<std::string, std::queue<std::unique_ptr<Message>>>
      fMessages;
  std::condition_variable fMessagesCond;
  std::mutex fQueueLock;
  bool fStopProcessing = false;
};

struct Publisher {
  Publisher(Broker &aBroker) : fBroker(aBroker) {}
  void publish(std::string aTopic, std::unique_ptr<Message> aMsg);

private:
  Broker &fBroker;
};
} // namespace ry
