#include "Server.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using networking::Server;
using networking::Connection;
using networking::Message;

struct MessageResult {
  std::string result;
  std::vector<Connection> sendTo;
  bool shouldShutdown;
};

namespace ServerAction {

json createJSONMessage(std::string type, std::string message);

json joinGame(const std::string roomCode, const Connection& connection, std::vector<Connection>& recipients);

json quitGame(const json& data, const Connection& connection, std::vector<Connection>& recipients, Server& server);

json createGame(std::string gameRules, const Connection& connection, std::vector<Connection>& recipients);

json sendChat(std::string message, const uintptr_t& senderID, std::vector<Connection>& recipients);

void shutdown();

void closeGame(Server& server, const Connection& connection);

}