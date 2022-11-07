#include "Server.h"
#include "ServerActions.h"
#include "controller.h"

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


void onConnect(Connection c) {
  spdlog::info("New Connection Found: {}", c.id);
}

void onDisconnect(Connection c) {
  spdlog::info("Connection Lost: {}", c.id);
}

std::deque<Message>
buildOutgoing(const std::string& log, std::vector<Connection> clients) {
  std::deque<Message> outgoing;
  for (auto client : clients) {
    outgoing.push_back({client, log});
  }
  return outgoing;
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

void initLogging() {
  spdlog::set_pattern("[%Y-%m-%d %H:%M] [Process: %P] [%^%l%$] %s:%# - %v");
  spdlog::info("starting program");
}

int main(int argc, char* argv[]) {
  initLogging();

  if (argc < 3) {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n";
    return 1;
  }

  unsigned short port = std::stoi(argv[1]);
  Server server{port, getHTTPMessage(argv[2]),  onConnect, onDisconnect};
  Controller roomManager;
  
  bool quit = false;
  while (!quit) {
    try {
      server.update();
    } catch (std::exception& e) {
      spdlog::error("Exception from Server update:\n{}\n\n", e.what());
      quit = true;
    }

    auto incoming = server.receive();
    auto [log, roomClients, quit] = ServerAction::processMessages(server, incoming, roomManager);
    auto outgoing = buildOutgoing(log, roomClients);
    server.send(outgoing);

    sleep(1);
  }

  return 0;
}
