#pragma once

#include "ClientActionHandler.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Client {

class MessageProcessor {
  public:
    MessageProcessor() { }

    std::string processMessage(const std::string& message); // expect string to be in json format

  private:
    Client::ActionHandler actionHandler;
};

}
