#include "Server.h"
#include "NetworkMessage.h"
#include "NetworkingHelper.h"
#include "handler.h"

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
json gameRules;


void onConnect(Connection c) {
  spdlog::info("New Connection Found: {}", c.id);
  clients.push_back(c);
}


void onDisconnect(Connection c) {
  spdlog::info("Connection Lost: {}", c.id);

  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));

  auto roomLoc = rooms.find(clientInfo.find(c.id)->second);
  std::vector<Connection> &roomClients = roomLoc->second;
  spdlog::info("Remaining Players in Room: {}", roomClients.size());
  
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

json joinGame(const std::string roomCode, const Connection& connection, std::vector<Connection>& recipients) {
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

json quitGame(const json& data, const Connection& connection, std::vector<Connection>& recipients, Server& server) {
  std::string playerDisconnected = std::string("Player Left");
  //   recieveMessage(playerDisconnected);
  server.disconnect(connection);

  json response = createJSONMessage("Player Left", connection.id + ": Has left");
  return response;
}

void shutdown() {
  spdlog::info("Shutting Down");
}

json createGame(std::string gameRules, const Connection& connection, std::vector<Connection>& recipients) {
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

void closeGame(Server& server, const Connection& connection) {
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

json sendChat(std::string message, const uintptr_t& senderID, std::vector<Connection>& recipients) {
  std::string roomCode = clientInfo.at(senderID);
  recipients = rooms.at(roomCode);
  
  std::ostringstream s;
  s << senderID << "> " << message;

  json response = createJSONMessage("chat", s.str());
  spdlog::info(s.str());

  return response;
}

MessageResult processMessages(Server& server, const std::deque<Message>& incoming) {
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
      json response = joinGame(roomcode, sender, recipients);
      result << response.dump();
    }
    else if (command == messageType.CREATE) {
      std::string gameRules {data["message"]};
      json response = createGame(gameRules, sender, recipients);
      result << response.dump();
    }
    else if (command == messageType.CLOSE_GAME) {
      closeGame(server, message.connection);
    }
    else {
      std::string message {data["message"]};
      json response = sendChat(message, sender.id, recipients);
      result << response.dump();
    }
  }
  
  return MessageResult{result.str(), recipients, quit};
}

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
