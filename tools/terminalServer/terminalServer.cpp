#include "Server.h"
#include "NetworkMessage.h"
#include "NetworkingHelper.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <nlohmann/json.hpp>
#include "handler.h"

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
json gameRules;


void onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  clients.push_back(c);
}


void onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));
  auto roomLoc = rooms.find(clientInfo.find(c.id)->second);
  std::vector<Connection> &roomClients = roomLoc->second;
  std::cout << roomClients.size() << "\n";
  auto removeLoc = std::remove_if(roomClients.begin(), roomClients.end(), [&c](const Connection client)
                              { return client.id == c.id; });
  roomClients.erase(removeLoc, roomClients.end());
  
  auto it = clientInfo.find(c.id);
  clientInfo.erase(it);
  
  std::cout << roomClients.size() << "\n";
  if(roomClients.size() == 0){
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

json createJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

json joinGame(const json& data, const auto& message, std::vector<Connection>& sendTo) {
  const std::string roomCode = data["message"];
  auto roomClients = rooms.find(roomCode);

  json response = createJSONMessage("Error", "wrong code");
  if (roomClients != rooms.end()) {
    roomClients->second.push_back(message.connection);
    clientInfo.insert(std::pair<uintptr_t, std::string> (message.connection.id, roomCode));

    // tell handler player joined
    std::string handlerInput ("Player Joined");
    recieveMessage(handlerInput);

    response = createJSONMessage("Success", "successfully joined");
  }

  sendTo.push_back(message.connection);
  return response;
}

void shutdown() {
  std::cout << "Shutting down.\n";
}

json createGame(const json& data, const auto& message, std::vector<Connection>& sendTo) {
  std::string gameRules = data["message"];
  json response;
  if (isJSON(gameRules)) {
      std::cout << "Game rules are valid JSON" << std::endl;

      json gamerule = completeParse(gameRules); //Send this json object to lower layers (when completed)

      // //Printing game rules keys to see if everything is parsed correctly when function is complete
      // std::cout << "Printing keys for game rules..." << std::endl;
      // for (auto it = gamerule.begin(); it != gamerule.end(); ++it)
      // {
      //     std::cout << "key: " << it.key() << '\n';
      // }

      hosts.push_back(message.connection);

      std::vector<Connection> roomClients {message.connection};
      std::string roomCode = randomCode();
      rooms.insert(std::pair<std::string, std::vector<Connection>> (roomCode, roomClients));
      clientInfo.insert(std::pair<uintptr_t, std::string> (message.connection.id, roomCode));

      response = createJSONMessage("Success", "successfully created - code: " + roomCode);
      sendTo.push_back(message.connection);

      //Tell handler that new game is created
      std::string handlerInput = std::string("Game Created");
      recieveMessage(handlerInput);
  } else {
      std::string error = "Error: Game rules are not in valid json format...";
      std::cout << error << std::endl;
      response = createJSONMessage("Error", error);

      sendTo.push_back(message.connection);
  }

  return response;
}

void closeGame(Server& server ,const auto& message) {
  bool messageSentbyHost = false;
  for (auto& host : hosts) {
    if (host.id == message.connection.id) {
      messageSentbyHost = true;
    }
  }

  if (messageSentbyHost) {
    std::string roomCode = clientInfo.at(message.connection.id);
      auto roomClients = rooms.at(roomCode);
      for(auto client: roomClients) {
          server.disconnect(client);
      }

      server.disconnect(message.connection);
      rooms.erase(roomCode);

      //Tell handler that a game ended
      std::string handlerInput = std::string("Game Ended");
      recieveMessage(handlerInput);
  }
}

json sendChat(const json& data, const auto& message, std::vector<Connection>& sendTo) {
  std::string roomCode = clientInfo.at(message.connection.id);
  sendTo = rooms.at(roomCode);
  
  std::ostringstream s;
  s << message.connection.id << "> " << data["message"];

  json response = createJSONMessage("chat", s.str());
  std::cout << s.str() << std::endl;

  return response;
}

MessageResult processMessages(Server& server, const std::deque<Message>& incoming) {
  std::ostringstream result;
  std::vector<Connection> sendTo;
  messageType messageType;
  
  bool quit = false;
  for (auto& message : incoming) {
    json data = json::parse(message.text);

    std::cout << data.dump() << std::endl;

    if (data["type"] == messageType.QUIT) {
      server.disconnect(message.connection);
    } 
    else if (data["type"]  == messageType.SHUTDOWN) {
      shutdown();
      quit = true;
    }
    else if (data["type"] == messageType.JOIN) {
      json response = joinGame(data, message, sendTo);
      result << response.dump();
    }
    else if (data["type"] == messageType.CREATE) {
      json response = createGame(data, message, sendTo);
      result << response.dump();
    }
    else if (data["type"] == messageType.CLOSE_GAME) {
      closeGame(server, message);
    }
    else if (data["type"] == messageType.QUIT) {
      std::string playerDisconnected = std::string("Player Left");
      //   recieveMessage(playerDisconnected);
    }
    else {
      json response = sendChat(data, message, sendTo);
      result << response.dump();
    }
  }
  
  return MessageResult{result.str(), sendTo, quit};
}

std::string
getHTTPMessage(const char* htmlLocation) {
  if (access(htmlLocation, R_OK ) != -1) {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};

  } else {
    std::cerr << "Unable to open HTML index file:\n"
              << htmlLocation << "\n";
    std::exit(-1);
  }
}

int main(int argc, char* argv[]) {

  //Initialize google logging
  //initLogging();

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
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      errorWhileUpdating = true;
    }

    auto incoming = server.receive();
    auto [log, roomClients, shouldQuit] = processMessages(server, incoming);
    auto outgoing = buildOutgoing(log, roomClients);
    server.send(outgoing);

    if (shouldQuit || errorWhileUpdating) {
      break;
    }

    sleep(1);
  }

  return 0;
}
