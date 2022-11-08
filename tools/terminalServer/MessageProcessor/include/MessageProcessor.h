#pragma once

#include "Server.h"
#include "ActionHandler.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using networking::Server;
using networking::Connection;
using networking::Message;

const std::unordered_map<std::string, ActionType> commandToAction = {
  {"Join", ActionType::join},
  {"Create", ActionType::create_game},
  {"close game", ActionType::end_game},
  {"Quit", ActionType::quit},
  {"Send chat", ActionType::send_chat}
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
