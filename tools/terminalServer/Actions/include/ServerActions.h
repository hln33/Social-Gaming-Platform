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

MessageResult processMessages(
    Server& server, 
    const std::deque<Message>& incoming,
    std::map<std::string, std::vector<Connection>>& rooms,
    std::map<uintptr_t, std::string>& clientInfo,
    std::vector<Connection>& hosts
); 

}
