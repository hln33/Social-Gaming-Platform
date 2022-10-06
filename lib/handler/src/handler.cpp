#include "handler.h"
#include <iostream>

void log(const std::string& text) {
    std::cout << text << std::endl;
}

// checks if a given string is valid JSON
bool isJSON(const std::string& text) {
    // this function can most likely just call https://json.nlohmann.me/api/basic_json/accept/. Just currently waiting on Bikram to integrate the library into the project
    // something like this:
    // return nlohmann::json::accept(text)

    return false; //placeholder return statement
}

bool MessageContains(const std::string& string, const std::string& subString) {
    return string.find(subString) != std::string::npos;
}

// comments below contain pseudo-code
void performBusinessLogic(const std::string& message) {
    if (MessageContains(message, "Player Left")) {
        // std::string playerName = getPlayerName(message);
        // businesslogic::removePlayer(playerName);
        log("A player has left");
    } else if (MessageContains(message, "Player Joined")) {
        // std::string playerName = getPlayerName(message);
        // businesslogic::addPlayer(playerName);
        log("A player has joined");
    } else if (MessageContains(message, "Game Ended")) {
        // businesslogic::endGame();
        log("A game has ended");
    } else if (MessageContains(message, "Game Created")) {
        // Configuration config = parseJSON(message);
        // businesslogic::createGame(config);
        log("A game has been created");
    } else {
        log("invalid message");
        throw std::runtime_error("invalid message passed to handler");
    }
}

// recieves message from networking
// For now, input will be assumed to be a string
void recieveMessage(std::string& message) {
    try {
        performBusinessLogic(message);
    } catch (std::exception& e) {
        log("call to business logic failed:");
        log(e.what());
    }
}