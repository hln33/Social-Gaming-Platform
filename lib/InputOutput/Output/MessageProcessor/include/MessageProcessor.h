#pragma once

#include "Server.h"
#include "ActionHandler.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using networking::Server;
using networking::Connection;
using networking::Message;


class MessageProcessor {
  public:
    MessageProcessor() { }

    json processMessage(const json& message);

  private:
    ActionHandler actionHandler;
};
