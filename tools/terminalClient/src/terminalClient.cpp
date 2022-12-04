/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////

#include "clientHelper.h"
#include "ClientMessageProcessor.h"


int main(int argc, char* argv[]) {
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
    createRoom(&client);
  }
  else if(input == "2"){
    joinRoom(&client);
  }
  else if(input == "0"){
    exit(0);
  }

  bool done = false;
  auto onTextEntry = [&done, &client] (std::string text) {
    if ("exit" == text || "quit" == text) {
      done = true;
    } else{
      json input;
      input["input"] = text;
      json payload = createJSONMessage("chat", input.dump());
      client.send(payload.dump());
    }
  };
  
  ChatWindow chatWindow(onTextEntry);
  Client::MessageProcessor messageProcessor;
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
      auto parsedMessage = messageProcessor.process(response);
      chatWindow.displayText(parsedMessage + "\n");

      if (messageProcessor.error) {
        chatWindow.displayText("Disconnecting...");
        chatWindow.update();
        client.disconnect();
      }
    }
    chatWindow.update();
  }

  return 0;
}