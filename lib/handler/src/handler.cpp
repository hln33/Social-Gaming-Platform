#include "handler.h"
#include <iostream>

// recieves message from networking
// For now, input will be assumed to be a string
void recieveMessage(std::string& message) {
    if (isJSON(message)) {
        // parseJSON(message)
        return;
    }

    if (MessageContains(message, "Player Left")) {
        log("A player has left");
    } else if (MessageContains(message, "Player Joined")) {
        log("A player has joined");
    } else if (MessageContains(message, "Game Ended")) {
        log("A game has ended");
    } else if (MessageContains(message, "Game Created")) {
        log("A game has been created");
    } else {
        log("invalid message");
    }
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

void log(const std::string& text) {
    std::cout << text << std::endl;
}