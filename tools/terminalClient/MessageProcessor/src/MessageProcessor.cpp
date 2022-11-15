// #include "MessageProcessor.h"
// #include "MessageProcessor_pi.h"

// #include <spdlog/spdlog.h>

// Outgoing MessageProcessor::processMessages(const std::deque<Message>& incoming) {
//   std::ostringstream result;
  
//   bool quit = false;
//   clearRecipients();

//   for (auto& message : incoming) {
//     json parsedMessage = json::parse(message.text);

//     const std::string command = parsedMessage["type"];
//     const json data = parsedMessage["message"]; 
//     Connection sender = message.connection;
//     json response = actionHandler.executeAction(command, data, sender);
    
//     result << response.dump();
//   }
  
//   return Outgoing{result.str(), this->recipients, quit};
// }