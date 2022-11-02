#include "Server.h"
#include "NetworkMessage.h"
#include "NetworkingHelper.h"
#include "handler.h"
#include "serverActions.h"

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

json quitGame(const json& data,
    const Connection& connection,
    std::vector<Connection>& recipients,
    Server& server) 
{
  std::string playerDisconnected = std::string("Player Left");
  //   recieveMessage(playerDisconnected);
  server.disconnect(connection);

  json response = createJSONMessage("Player Left", connection.id + ": Has left");
  return response;
}

void shutdown() {
  spdlog::info("Shutting Down");
}

json joinGame(
    const std::string& roomCode,
    const Connection& connection,
    std::vector<Connection>& recipients,
    std::map<std::string, std::vector<Connection>>& rooms,
    std::map<uintptr_t, std::string>& clientInfo) 
{
  auto roomClients = rooms.find(roomCode);

  json response = createJSONMessage("Error", "wrong code");
  if (roomClients != rooms.end()) {
    roomClients->second.push_back(connection);
    clientInfo.insert(std::pair<uintptr_t, std::string> (connection.id, roomCode));

    // tell handler player joined
    std::string handlerInput ("Player Joined");
    recieveMessage(handlerInput);

    response = createJSONMessage("Success", "successfully joined");
  }

  recipients.push_back(connection);
  return response;
}

json createGame(
    std::string gameRules,
    const Connection& connection,
    std::vector<Connection>& recipients,
    std::map<std::string, std::vector<Connection>>& rooms,
    std::vector<Connection>& hosts,
    std::map<uintptr_t, std::string>& clientInfo)
{
  json response;
  if (isJSON(gameRules)) {
      spdlog::info("Game rules are valid JSON");

      json gamerule = completeParse(gameRules); //Send this json object to lower layers (when completed)

      // //Printing game rules keys to see if everything is parsed correctly when function is complete
      // std::cout << "Printing keys for game rules..." << std::endl;
      // for (auto it = gamerule.begin(); it != gamerule.end(); ++it)
      // {
      //     std::cout << "key: " << it.key() << '\n';
      // }

      hosts.push_back(connection);

      std::vector<Connection> roomClients {connection};
      std::string roomCode = randomCode();
      rooms.insert(std::pair<std::string, std::vector<Connection>> (roomCode, roomClients));
      clientInfo.insert(std::pair<uintptr_t, std::string> (connection.id, roomCode));

      response = createJSONMessage("Success", "successfully created - code: " + roomCode);
      
      //Tell handler that new game is created
      std::string handlerInput = std::string("Game Created");
      recieveMessage(handlerInput);
  } else {
      std::string error = "Error: Game rules are not in valid json format...";
      spdlog::error(error);
      response = createJSONMessage("Error", error);

  }
  recipients.push_back(connection);

  return response;
}

void closeGame(
    Server& server,
    const Connection& connection,
    std::map<uintptr_t, std::string>& clientInfo,
    std::vector<Connection>& hosts,
    std::map<std::string, std::vector<Connection>>& rooms) 
{
  auto isHost = [connection](const auto& host) { return connection.id == host.id; };
  bool messageSentbyHost = std::find_if(hosts.begin(), hosts.end(), isHost) != hosts.end();
  if (!messageSentbyHost) { 
    return; 
  }

  std::string roomCode = clientInfo.at(connection.id);
  auto roomClients = rooms.at(roomCode);
  for (auto client: roomClients) {
      server.disconnect(client);
  }

  server.disconnect(connection);
  rooms.erase(roomCode);

  //Tell handler that a game ended
  std::string handlerInput = std::string("Game Ended");
  recieveMessage(handlerInput);
}

json sendChat(
    std::string& message, 
    const uintptr_t& senderID, 
    std::vector<Connection>& recipients,
    std::map<uintptr_t, std::string>& clientInfo,
    std::map<std::string, std::vector<Connection>>& rooms) 
{
  std::string roomCode = clientInfo.at(senderID);
  recipients = rooms.at(roomCode);
  
  std::ostringstream s;
  s << senderID << "> " << message;

  json response = createJSONMessage("chat", s.str());
  spdlog::info(s.str());

  return response;
}

MessageResult processMessages(
    Server& server, 
    const std::deque<Message>& incoming,
    std::map<std::string, std::vector<Connection>>& rooms,
    std::map<uintptr_t, std::string>& clientInfo,
    std::vector<Connection>& hosts) 
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
      quitGame(data, sender, recipients, server);
    } 
    else if (command == messageType.SHUTDOWN) {
      shutdown();
      quit = true;
    }
    else if (command == messageType.JOIN) {
      const std::string roomcode {data["message"]};
      json response = joinGame(roomcode, sender, recipients, rooms, clientInfo);
      result << response.dump();
    }
    else if (command == messageType.CREATE) {
      std::string gameRules {data["message"]};
      json response = createGame(gameRules, sender, recipients, rooms, hosts, clientInfo);
      result << response.dump();
    }
    else if (command == messageType.CLOSE_GAME) {
      closeGame(server, message.connection, clientInfo, hosts, rooms);
    }
    else {
      std::string message {data["message"]};
      json response = sendChat(message, sender.id, recipients, clientInfo, rooms);
      result << response.dump();
    }
  }
  
  return MessageResult{result.str(), recipients, quit};
}