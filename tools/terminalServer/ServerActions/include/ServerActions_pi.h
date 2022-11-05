#pragma once

// this file contains function declarations for test.cpp to call
#include "ServerActions.h"

json createJSONMessage(std::string type, std::string message);

json quitGame(const Connection& connection); 

json joinGame(
    const std::string& roomCode,
    const Connection& connection,
    std::vector<Connection>& recipients,
    ServerAction::ServerDetails& serverDetails
);

json createGame(
    std::string& gameRules,
    const Connection& connection,
    std::vector<Connection>& recipients,
    ServerAction::ServerDetails& serverDetails
);