#include "handler.h"
#include <glog/logging.h>
#include <../json.hpp>
#include <iostream>

using json = nlohmann::json;
// checks if a given string is valid JSON
bool isJSON(const std::string& text) {
    // this function can most likely just call https://json.nlohmann.me/api/basic_json/accept/. Just currently waiting on Bikram to integrate the library into the project
    // something like this:
    // return nlohmann::json::accept(text)
    if(text)
        return accept(text);
    // return false; placeholder return statement
}

bool MessageContains(const std::string& string, const std::string& subString) {
    return string.find(subString) != std::string::npos;
}

// comments below contain pseudo-code
void performBusinessLogic(const std::string& message) {
    if (MessageContains(message, "Player Left")) {
        // std::string playerName = getPlayerName(message);
        // businesslogic::removePlayer(playerName);
        LOG(INFO) << "A player has left";
    } else if (MessageContains(message, "Player Joined")) {
        // std::string playerName = getPlayerName(message);
        // businesslogic::addPlayer(playerName);
        LOG(INFO) << "A player has joined";
    } else if (MessageContains(message, "Game Ended")) {
        // businesslogic::endGame();
        LOG(INFO) << "A game has ended";
    } else if (MessageContains(message, "Game Created")) {
        // Configuration config = parseJSON(message);
        // businesslogic::createGame(config);
        LOG(INFO) << "A game has been created";
    } else {
        LOG(ERROR) << "invalid message";
        throw std::runtime_error("invalid message passed to handler");
    }
}

// recieves message from networking
// For now, input will be assumed to be a string
void recieveMessage(std::string& message) {
    google::InitGoogleLogging("Handler");

    try {
        performBusinessLogic(message);
    } catch (std::exception& e) {
        LOG(ERROR) << "call to business logic failed:";
        LOG(ERROR) << e.what();
    }
}