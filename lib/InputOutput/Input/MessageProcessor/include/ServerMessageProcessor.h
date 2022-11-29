#pragma once

#include "Server.h"
#include "ServerActionHandler.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using networking::Connection;
using networking::Message;

struct Outgoing {
  std::string result;
  std::set<Connection> sendTo;
  bool shouldQuit;
};

namespace Server {

class MessageProcessor {
  public:
    MessageProcessor() { }

    Outgoing processMessages(const std::deque<Message>& incoming);

  private:
    Server::ActionHandler actionHandler;
    std::set<Connection> recipients;

    void clearRecipients() {
      recipients.clear();
    }
};

}
