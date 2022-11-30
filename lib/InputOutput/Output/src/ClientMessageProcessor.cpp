#include "ClientMessageProcessor.h"

#include <spdlog/spdlog.h>


std::string Client::MessageProcessor::processMessage(const std::string& message) {
  json jsonMessage = json::parse(message);
  const std::string command = jsonMessage["type"];
  const std::string data = jsonMessage["message"];

  std::string result = actionHandler.executeAction(command, data);

  return result;
}