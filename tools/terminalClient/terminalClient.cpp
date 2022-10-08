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

using json = nlohmann::json;

json createJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

int
main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: \n  " << argv[0] << " <ip address> <port>\n"
              << "  e.g. " << argv[0] << " localhost 4002\n";
    return 1;
  }

  networking::Client client{argv[1], argv[2]};
  bool done = false;
  bool auth = false;
  auto onTextEntry = [&done, &client, &auth] (std::string text) {
    if ("exit" == text || "quit" == text) {
      done = true;
    } else if(auth) {
      json payload = createJSONMessage("chat", text);
      client.send(payload.dump());
    }
    else {
      json payload = createJSONMessage("join", text);
      client.send(payload.dump());
    }
  };
  //Client joins, Client will be prompted to enter a code.
  //if password is correct then "auth" variable will be set to true.
  ChatWindow chatWindow(onTextEntry);
  chatWindow.displayText("Please enter the code\n");
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
      if(data["type"] == "success"){
        auth = true;
      }
      std::string message = data["message"];
      chatWindow.displayText(message + "\n");
    }
    chatWindow.update();
  }

  return 0;
}