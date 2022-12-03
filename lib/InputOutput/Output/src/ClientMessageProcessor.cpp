#include "ClientMessageProcessor.h"
#include "Response.h"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using json = nlohmann::json;


std::string Client::MessageProcessor::process(const std::string& message) {
  json jsonMessage = json::parse(message);
  const std::string command = jsonMessage["type"];
  const std::string data = jsonMessage["message"];

  std::string result = actionHandler.executeAction(command, data);
  if (command == ResponseCode::ERROR || command == ResponseCode::JOIN_GAME_FAIL) {
    error = true;
  }
  return result;
}