/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <unistd.h>
#include <fstream>

#include "clientWindow.h"
#include "Client.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

json createJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

std::string printMenu(){
  std::stringstream ss;
  ss << "------MENU------- \n";
  ss << "1 - Create \n";
  ss << "2 - Join \n";
  ss << "0 - To close game \n";

  return ss.str();
}

int
main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: \n  " << argv[0] << " <ip address> <port>\n"
              << "  e.g. " << argv[0] << " localhost 4002\n";
    return 1;
  }
  networking::Client client{argv[1], argv[2]};
  try {
      client.update();
  } catch (std::exception& e) {
    std::cout << "Exception from Client update: \n";
    std::cout << e.what();
    exit(0);
  }
  std::string input;
  std::cout << printMenu();
  std::cin >> input;

  if(input == "1"){
    std::cout << "Enter path to JSON file \n";
    std::string filePath;
    std::cin.ignore();
    std::getline(std::cin, filePath);

    std::ifstream file(filePath);
    while(!file.is_open()) {
      std::cout << "failed to open " << filePath << '\n' << "Enter path to JSON file \n";
      std::cin.ignore();
      std::getline(std::cin, filePath);
    }
    json gameRules = json::parse(file);
    json payload = createJSONMessage("Create", gameRules.dump());
    client.send(payload.dump());
  
  }
  else if(input == "2"){
    std::cout << "Enter Code \n";
    std::string code;
    std::cin >> code;
    json payload = createJSONMessage("Join", code);
    client.send(payload.dump());
  }
  else if(input == "0"){
    exit(0);
  }

  bool done = false;
  auto onTextEntry = [&done, &client] (std::string text) {
    if ("exit" == text || "quit" == text) {
      done = true;
    } else{
      json payload = createJSONMessage("chat", text);
      client.send(payload.dump());
    }
  };
  
  ChatWindow chatWindow(onTextEntry);
  while (!done && !client.isDisconnected()) {
    try {
      client.update();
    } catch (std::exception& e) {
      chatWindow.displayText("Exception from Client update:");
      chatWindow.displayText(e.what());
      done = true;
    }
    auto response = client.receive();
    if (!response.empty()) {
      json data = json::parse(response);
      std::string message = data["message"];
      chatWindow.displayText(message + "\n");
    }
    chatWindow.update();
  }

  return 0;
}