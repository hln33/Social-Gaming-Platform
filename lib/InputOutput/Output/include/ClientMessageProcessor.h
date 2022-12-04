#pragma once

#include "ClientActionHandler.h"


namespace Client {

class MessageProcessor {
  public:
    MessageProcessor() { }

    bool error = false;
    std::string process(const std::string& message); // expect string to be in json format

  private:
    Client::ActionHandler actionHandler;
};

}
