#include "MessageProcessor.h"

#include <spdlog/spdlog.h>


json MessageProcessor::processMessages(const json message) {
  std::ostringstream result;

  json response = actionHandler.executeAction(command, parsedData);
  result << response.dump();  
}