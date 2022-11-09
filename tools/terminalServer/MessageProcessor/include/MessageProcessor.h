#pragma once

#include "Server.h"
#include "ActionHandler.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using networking::Server;
using networking::Connecti{"Send chat", ActionType::send_chat}
};

struct Outgoing {
  std::string result;
  std::vector<Connection> sendTo;
  bool shouldShutdown;
};

class MessageProcessor {
  public:
    MessageProcessor() { }

    Outgoing processMessages(const std::deque<Message>& incoming);

  private:
    ActionHandler actionHandler;
    std::vector<Connection> recipients;
    std::ostringstream messageLog;

    void clearRecipients() {
      recipients.clear();
    }
};
