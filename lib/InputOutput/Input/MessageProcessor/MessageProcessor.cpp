#include "MessageProcessor.h"

#include <spdlog/spdlog.h>


Outgoing MessageProcessor::processMessages(const std::deque<Message>& incoming) {
  std::ostringstream result;
  
  bool quit = false;
  clearRecipients();

  for (auto& message : incoming) {
    json parsedMessage = json::parse(message.text);

    const std::string command = parsedMessage["type"];
    const std::string data = parsedMessage["message"]; 

    json parsedData = json::parse(data);    

    Connection sender = message.connection;
    json response = actionHandler.executeAction(command, parsedData, sender, this->recipients);


    result << response.dump();
  }
  
  return Outgoing{result.str(), this->recipients, quit};
}