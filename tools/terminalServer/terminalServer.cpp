#include "Server.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <nlohmann/json.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using json = nlohmann::json;
using networking::Server;
using networking::Connection;
using networking::Message;

std::vector<Connection> clients;
json gameRules;
std::string code = "";
using networking::Server;
using networking::Connection;
using networking::Message;


std::string randomCode(){
  std::string chars(
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890");


    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
    std::string code = "";
    for(int i = 0; i < 6; ++i) {
        code.append(std::string(1,chars[index_dist(rng)]));
    }

    return code;
}


void onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  //clients.push_back(c);
}


void onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));
}


struct MessageResult {
  std::string result;
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
  bool quit = false;
  for (auto& message : incoming) {
    json data = json::parse(message.text);
    if (data["type"] == "quit") {
      server.disconnect(message.connection);
    } else if (data["type"]  == "shutdown") {
      std::cout << "Shutting down.\n";
       quit = true;
    }
    else if(data["type"] == "join"){
      std::deque<Message> outgoing;
      if(code == data["message"]){
        clients.push_back(message.connection);
        json response = createJSONMessage("success", "successfully joined");
        outgoing.push_back({message.connection, response.dump()});
        server.send(outgoing);
      }
      else{
        json response = createJSONMessage("error", "wrong code");
        outgoing.push_back({message.connection, response.dump()});
        server.send(outgoing);
      }
    }
    else{
      std::ostringstream s;
      s << message.connection.id << "> " << data["message"];
      json response = createJSONMessage("chat", s.str());
      result << response.dump();
    }
  }
  return MessageResult{result.str(), quit};
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
  if (argc < 4) {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <json> <html response>\n";
    return 1;
  }
  std::ifstream f(argv[2]);
  gameRules = json::parse(f);
  code = randomCode();
  unsigned short port = std::stoi(argv[1]);
  Server server{port, getHTTPMessage(argv[3]),  onConnect, onDisconnect};
  
  std::cout << "Your Code: " << code << "\n";
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
    auto [log, shouldQuit] = processMessages(server, incoming);
    auto outgoing = buildOutgoing(log, clients);
    server.send(outgoing);

    if (shouldQuit || errorWhileUpdating) {
      break;
    }

    sleep(1);
  }

  return 0;
}