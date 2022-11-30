#pragma once

#include "ClientActionHandler.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Client {

class MessageProcessor {
  public:
    MessageProcessor() { }

    json processMessage(const json& message);

  private:
    Client::ActionHandler actionHandler;
};

}
