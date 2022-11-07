#include "MessageProcessor.h"
#include "MessageProcessor_pi.h"

#include <spdlog/spdlog.h>

using json = nlohmann::json;
using networking::Server;
using networking::Connection;
using networking::Message;


json createJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

json MessageProcessor::quitGame(const Connection& connection) {
  // roomCode = roomManager.getRoomCode(connection);
  // roomManager.leaveRoom(roomCode, connection)
  return createJSONMessage("Quit", "Player has left");
}

json MessageProcessor::shutdown() {
  spdlog::info("Shutting Down");

  return createJSONMessage("shutdown", "server shutting down");
}

json MessageProcessor::joinGame(const std::string& roomCode, const Connection& connection) {
  //roomManager.joinRoom(roomCode, nullptr);

  return createJSONMessage("Player joined", "");
}

json MessageProcessor::createGame(std::string& gameRules, const Connection& host) {
  //roomManager.createRoom(gameRules, host);

  return createJSONMessage("Game created", "");
}

json MessageProcessor::closeGame(const Connection& connection) {
  // check if sender is allowed to end the game (i.e. if they are a host)

  // if so, find all connections in their room and disconnect them

    // for (auto client: roomClients) {
    //     server.disconnect(client);
    // }

  // remove the room from roomManager
    // this->roomManager.removeRoom(roomCode)

    return createJSONMessage("close game", "game ended");
}

json MessageProcessor::sendChat(std::string& message, const uintptr_t& senderID) {
  
  // roomCode = roomManager.getRoomCode(sender);
  // reciprients = roomManager.getPlayers(roomCode);
  
  std::ostringstream s;
  s << senderID << "> " << message;

  json response = createJSONMessage("chat", s.str());
  spdlog::info(s.str());

  return response;
}

Outgoing MessageProcessor::processMessages(const std::deque<Message>& incoming) {
  std::ostringstream result;
  
  bool quit = false;
  clearRecipients();
  for (auto& message : incoming) {
    json data = json::parse(message.text);
    Connection sender = message.connection;
    json response;

    spdlog::info(data["type"]);
    GameAction command = commandToAction.at(data["type"]);
    switch(command) {
      case QUIT: {
        response = quitGame(sender);
        break;
      }
      case SHUTDOWN: {
        json response = shutdown();
        quit = true;
        break;
      }
      case JOIN: {
        const std::string roomcode {data["message"]};
        response = joinGame(roomcode, sender);
        break;
      }
      case CREATE: {
        std::string gameRules {data["message"]};
        response = createGame(gameRules, sender);
        break;
      }
      case CLOSE_GAME: {
        closeGame(message.connection);
        break;
      }
      case SEND_CHAT: {
        std::string message {data["message"]};
        response = sendChat(message, sender.id);
        break;
      }
      default:
        spdlog::error("unknown command!");
        response = createJSONMessage("unknwon command", "");
    };
    
    result << response.dump();
  }
  
  return Outgoing{result.str(), this->recipients, quit};
}