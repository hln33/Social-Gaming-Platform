#include "Server.h"
#include "NetworkMessage.h"
#include "NetworkingHelper.h"
#include "handler.h"
#include "ServerActions.h"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

using json = nlohmann::json;
using networking::Server;
using networking::Connection;
using networking::Message;


std::vector<Connection> clients;
std::vector<Connection> hosts; //Holds all hosts so that when they send close the game ends

//Clients From Room Code
std::map<std::string, std::vector<Connection>>  rooms;
//Client Info from ID
std::map<uintptr_t, std::string> clientInfo;  


void onConnect(Connection c) {
  spdlog::info("New Connection Found: {}", c.id);
  clients.push_back(c);
}

void onDisconnect(Connection c) {
  spdlog::info("Connection Lost: {}", c.id);

  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));

  // find room client was in
  auto roomLoc = rooms.find(clientInfo.find(c.id)->second);
  std::vector<Connection> &roomClients = roomLoc->second;
  spdlog::info("Remaining Players in Room: {}", roomClients.size());
  
  // remove client from room
  auto removeLoc = std::remove_if(roomClients.begin(), roomClients.end(), 
                      [&c](const Connection& client) { return client.id == c.id; });
  roomClients.erase(removeLoc, roomClients.end());
  
  auto it = clientInfo.find(c.id);
  if (it != clientInfo.end()) {
    clientInfo.erase(it);
  }

  std::cout << roomClients.size() << "\n";
  if(roomClients.size() == 0 && roomLoc != rooms.end()){
    rooms.erase(roomLoc);
  }
  for(auto it = rooms.cbegin(); it != rooms.cend(); ++it){
      std::cout << it->first << "\n";
  }
  for(auto it = clientInfo.cbegin(); it != clientInfo.cend(); ++it){
      std::cout << it->first << "\n";
  } 
}


struct MessageResult {
  std::string result;
  std::vector<Connection> sendTo;
  bool shouldShutdown;
};

std::deque<Message>
buildOutgoing(const std::string& log, std::vector<Connection> clients) {
  std::deque<Message> outgoing;
  for (auto client : clients) {
    outgoing.push_back({client, log});
  }
  return outgoing;
}


// MessageResult processMessages(Server& server, const std::deque<Message>& incoming) {
//   std::ostringstream result;
//   std::vector<Connection> recipients;
//   messageType messageType;
  
//   bool quit = false;
//   for (auto& message : incoming) {
//     json data = json::parse(message.text);
//     Connection sender = message.connection;
//     std::string command {data["type"]};

//     if (command == messageType.QUIT) {
//       ServerAction::quitGame(data, sender, recipients, server);
//     } 
//     else if (command == messageType.SHUTDOWN) {
//       ServerAction::shutdown();
//       quit = true;
//     }
//     else if (command == messageType.JOIN) {
//       const std::string roomcode {data["message"]};
//       json response = joinGame(roomcode, sender, recipients);
//       result << response.dump();
//     }
//     else if (command == messageType.CREATE) {
//       std::string gameRules {data["message"]};
//       json response = createGame(gameRules, sender, recipients);
//       result << response.dump();
//     }
//     else if (command == messageType.CLOSE_GAME) {
//       closeGame(server, message.connection);
//     }
//     else {
//       std::string message {data["message"]};
//       json response = sendChat(message, sender.id, recipients);
//       result << response.dump();
//     }
//   }
  
//   return MessageResult{result.str(), recipients, quit};
// }

std::string
getHTTPMessage(const char* htmlLocation) {
  if (access(htmlLocation, R_OK ) != -1) {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};

  } else {
    spdlog::error("Unable to open HTML index file:\n{}\n", htmlLocation);
    std::exit(-1);
  }
}

int main(int argc, char* argv[]) {
  spdlog::set_pattern("[%Y-%m-%d %H:%M] [Process: %P] [%^%l%$] %s:%# - %v");
  spdlog::info("starting program");

  if (argc < 3) {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n";
    return 1;
  }

  unsigned short port = std::stoi(argv[1]);
  Server server{port, getHTTPMessage(argv[2]),  onConnect, onDisconnect};
  
  while (true) {
    bool errorWhileUpdating = false;

    try {
      server.update();
    } catch (std::exception& e) {
      spdlog::error("Exception from Server update:\n{}\n\n", e.what());
      errorWhileUpdating = true;
    }

    auto incoming = server.receive();
    auto [log, roomClients, shouldQuit] = ServerAction::processMessages(server, incoming, rooms, clientInfo, hosts);
    auto outgoing = buildOutgoing(log, roomClients);
    server.send(outgoing);

    if (shouldQuit || errorWhileUpdating) {
      break;
    }

    sleep(1);
  }

  return 0;
}
