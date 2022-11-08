#pragma once

#include "Server.h"
#include "controller.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using networking::Server;
using networking::Connection;
using networking::Message;

enum GameAction {
  QUIT,
  SHUTDOWN,
  JOIN,
  CREATE,
  CLOSE_GAME,
  SEND_CHAT
};

const std::unordered_map<std::string, GameAction> commandToAction = {
  {"Join", JOIN},
  {"Create", CREATE},
  {"Close game", CLOSE_GAME},
  {"Quit", QUIT},
  {"Send chat", SEND_CHAT}
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
    Controller roomManager;
    std::vector<Connection> recipients;
    std::ostringstream messageLog;

    void clearRecipients() {
      recipients.clear();
    }

    // game actions
    json sendChat(std::string& message, const uintptr_t& senderID);
    json createGame(std::string& gameRules, const Connection& host);
    json joinGame(const std::string& roomCode, const Connection& connection);
    json quitGame(const Connection& connection);
    json shutdown();
    json closeGame(const Connection& connection); 
};
