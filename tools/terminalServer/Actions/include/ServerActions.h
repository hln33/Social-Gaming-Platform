#pragma once

#include "Server.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using networking::Server;
using networking::Connection;
using networking::Message;

namespace ServerAction {

struct MessageResult {
  std::string result;
  std::vector<Connection> sendTo;
  bool shouldShutdown;
};

// json createJSONMessage(std::string type, std::string message);

// json joinGame(
//     const std::string& roomCode,
//     const Connection& connection,
//     std::vector<Connection>& recipients,
//     std::map<std::string, std::vector<Connection>>& rooms,
//     std::map<uintptr_t, std::string>& clientInfo
// );
 
// json quitGame(const json& data, const Connection& connection, std::vector<Connection>& recipients, Server& server);

// json createGame(
//     std::string gameRules,
//     const Connection& connection,
//     std::vector<Connection>& recipients,
//     std::map<std::string, std::vector<Connection>>& rooms,
//     std::vector<Connection>& hosts,
//     std::map<uintptr_t, std::string>& clientInfo
// );

// void closeGame(
//     Server& server,
//     const Connection& connection,
//     std::map<uintptr_t, std::string>& clientInfo,
//     std::vector<Connection>& hosts,
//     std::map<std::string, std::vector<Connection>>& rooms
// ); 

// json sendChat(
//     std::string& message, 
//     const uintptr_t& senderID, 
//     std::vector<Connection>& recipients,
//     std::map<uintptr_t, std::string>& clientInfo,
//     std::map<std::string, std::vector<Connection>>& rooms
// ); 

// void shutdown();

// void closeGame(Server& server, const Connection& connection);

// }

MessageResult processMessages(
    Server& server, 
    const std::deque<Message>& incoming,
    std::map<std::string, std::vector<Connection>>& rooms,
    std::map<uintptr_t, std::string>& clientInfo,
    std::vector<Connection>& hosts
); 

}
