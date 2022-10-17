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
  //clients.push_back(c);
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

MessageResult processMessages(Server& server, const std::deque<Message>& incoming) {
  std::ostringstream result;
  std::deque<Message> outgoing;
  std::vector<Connection> sendTo;
  bool quit = false;
  for (auto& message : incoming) {
    json data = json::parse(message.text);

    if (data["type"] == messageType_to_string(Quit)) {
      server.disconnect(message.connection);
    } else if (data["type"]  == "shutdown") {
      std::cout << "Shutting down.\n";
       quit = true;
    }
    else if(data["type"] == messageType_to_string(Join)){
      const std::string roomCode = data["message"];
      auto roomClients = rooms.find(roomCode);
      if(roomClients  == rooms.end()){
        json response = createJSONMessage("Error", "wrong code");
        sendTo.push_back(message.connection);
        result << response.dump(); 
      }
      else{
        roomClients->second.push_back(message.connection);
        clientInfo.insert(std::pair<uintptr_t, std::string> (message.connection.id, roomCode));
        json response = createJSONMessage("Success", "successfully joined");
        sendTo.push_back(message.connection);
        result << response.dump();    
      }
    }
    else if(data["type"] == messageType_to_string(Create)){
      // pass json data to handler here
      
      std::string gameRules = data["message"];
      std::cout << gameRules;
      std::string roomCode = randomCode();
      std::vector<Connection> roomClients;
      roomClients.push_back(message.connection);
      
      hosts.push_back(message.connection);

      rooms.insert(std::pair<std::string, std::vector<Connection>> (roomCode, roomClients));
      clientInfo.insert(std::pair<uintptr_t, std::string> (message.connection.id, roomCode));
      json response = createJSONMessage("Success", "successfully created - code: " + roomCode);
      sendTo.push_back(message.connection);
      result << response.dump();

    }
    else{
      std::ostringstream s;
      std::string roomCode = clientInfo.at(message.connection.id);
      sendTo = rooms.at(roomCode);   
      s << message.connection.id << "> " << data["message"];
      json response = createJSONMessage("chat", s.str());
      std::cout << s.str() << "\n";
      result << response.dump();

      //To handle close request by host
      if (data["message"] == "close game") {
        bool closeRoom = false;

        for(auto host : hosts) {
          if (host.id == message.connection.id) {
            closeRoom = true;
          }
        }

        if (closeRoom) {
          std::string roomCode = clientInfo.at(message.connection.id);
          auto roomClients = rooms.at(roomCode);

          for(auto client: roomClients) 
              server.disconnect(client);

          server.disconnect(message.connection);
        }
      }
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
