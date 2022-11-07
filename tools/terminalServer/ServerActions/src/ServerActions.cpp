#include "Server.h"
#include "NetworkMessage.h"
#include "handler.h"

#include "ServerActions.h"
#include "ServerActions_pi.h"

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


json createJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

json quitGame(const Connection& connection, Controller& roomManager) {
  // roomCode = roomManager.getRoomCode(connection);
  // roomManager.leaveRoom(roomCode, connection)
}

void shutdown() {
  spdlog::info("Shutting Down");
}

json joinGame(
    const std::string& roomCode,
    const Connection& connection,
    std::vector<Connection>& recipients,
    Controller& roomManager) 
{
  //roomManager.joinRoom(roomCode, nullptr);
}

json createGame(
    std::string& gameRules,
    const Connection& host,
    std::vector<Connection>& recipients,
    Controller& roomManager)
{
  //roomManager.createRoom(gameRules, host);
}

void closeGame(
    Server& server,
    const Connection& connection,
    Controller& roomManager) 
{
  // check if sender is allowed to end the game (i.e. if they are a host)

  // if so, find all connections in their room and disconnect them

    // for (auto client: roomClients) {
    //     server.disconnect(client);
    // }

  // remove the room from roomManager
    // roomManager.removeRoom(roomCode)
}

json sendChat(
    std::string& message, 
    const uintptr_t& senderID, 
    std::vector<Connection>& recipients,
    Controller& roomManager) 
{
  
  // roomCode = roomManager.getRoomCode(sender);
  // reciprients = roomManager.getPlayers(roomCode);
  
  std::ostringstream s;
  s << senderID << "> " << message;

  json response = createJSONMessage("chat", s.str());
  spdlog::info(s.str());

  return response;
}

ServerAction::MessageResult ServerAction::processMessages(
    Server& server, 
    const std::deque<Message>& incoming,
    Controller& roomManager) 
{
  std::ostringstream result;
  std::vector<Connection> recipients;
  messageType messageType;
  
  bool quit = false;
  for (auto& message : incoming) {
    json data = json::parse(message.text);
    Connection sender = message.connection;
    std::string command {data["type"]};

    if (command == messageType.QUIT) {
      quitGame(sender, roomManager);
      server.disconnect(sender);
    } 
    else if (command == messageType.SHUTDOWN) {
      shutdown();
      quit = true;
    }
    else if (command == messageType.JOIN) {
      const std::string roomcode {data["message"]};
      json response = joinGame(roomcode, sender, recipients, roomManager);
      result << response.dump();
    }
    else if (command == messageType.CREATE) {
      std::string gameRules {data["message"]};
      json response = createGame(gameRules, sender, recipients, roomManager);
      result << response.dump();
    }
    else if (command == messageType.CLOSE_GAME) {
      closeGame(server, message.connection, roomManager);
    }
    else {
      std::string message {data["message"]};
      json response = sendChat(message, sender.id, recipients, roomManager);
      result << response.dump();
    }
  }
  
  return ServerAction::MessageResult{result.str(), recipients, quit};
}