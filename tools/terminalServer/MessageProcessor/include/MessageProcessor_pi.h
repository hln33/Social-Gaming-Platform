#pragma once

// this file contains function declarations for test.cpp to call
#include "MessageProcessor.h"

json createJSONMessage(std::string type, std::string message);

json quitGame(const Connection& connection, Controller& roomManager); 

json joinGame(
    const std::string& roomCode,
    const Connection& connection,
    std::vector<Connection>& recipients,
    Controller& roomManager
);

json createGame(
    std::string& gameRules,
    const Connection& connection, 
    std::vector<Connection>& recipients,
    Controller& roomManager
);