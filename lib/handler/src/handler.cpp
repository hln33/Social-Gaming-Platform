// #include <glog/logging.h>
#include <iostream>
#include "handler.h"
#include <glog/logging.h>
#include <../json.hpp>
#include <iostream>

enum class parse_event_t : std:: uint8_t{
    object_start,
    object_end,
    array_start,
    array_end,
    key,
    value
};

using json = nlohmann::json;

// checks if a given string is valid JSON
bool isJSON(const std::string& text) {
    return json::accept(text);
}

//function used to parse an inputted JSON file
//for now this is using a string to be consistent with the above check
//will have to wait for a test file/test info to run this code
json completeParse(const std::string& text){
    //we need to make sure things are parsed correctly
    try{
        json j_complete = json::parse(text);
        return j_complete;
    }
    catch (json::parse_error& e){
        std::cout << "message " << e.what() << '\n'
                  << "exception id: " << e.id << '\n'
                  << "byte postion of error: " << e.byte <<std::endl;
    };
    //refer to json.nlohmann.me/api/basic_json/parse/ for breakdown of parse
    // json::parser_callback_t cb = [](int depth, json::parse_event_t event, json& parsed){
    //     //this can be used to sort through the parser elements after reading
    //     if(event == json::parse_event_t::key and parsed == json("setup"))
    //     {
    //         return false;
    //     }
    //     else
    //     {
    //         return true;
    //     }
    // };
    // ^^This code should not be called with json::parse(text)
}

bool MessageContains(const std::string& string, const std::string& subString) {
    return string.find(subString) != std::string::npos;
}

// comments below contain pseudo-code
void performBusinessLogic(const std::string& message) {
    if (MessageContains(message, "Player Left")) {
        // std::string playerName = getPlayerName(message);
        // businesslogic::removePlayer(playerName);
        // LOG(INFO) << "A player has left";
        std::cout << "A player has left";
    } else if (MessageContains(message, "Player Joined")) {
        // std::string playerName = getPlayerName(message);
        // businesslogic::addPlayer(playerName);
        // LOG(INFO) << "A player has joined";
        std::cout << "A player has joined";
    } else if (MessageContains(message, "Game Ended")) {
        // businesslogic::endGame();
        // LOG(INFO) << "A game has ended";
        std::cout << "A game has ended";
    } else if (MessageContains(message, "Game Created")) {
        // Configuration config = parseJSON(message);
        // businesslogic::createGame(config);
        // LOG(INFO) << "A game has been created";
        std::cout << "A game has been created";
    } else {
        // LOG(ERROR) << "invalid message";
        std::cout << "invalid message";
        throw std::runtime_error("invalid message passed to handler");
    }
}

// recieves message from networking
// For now, input will be assumed to be a string
void recieveMessage(std::string& message) {
    // google::InitGoogleLogging("Handler");

    try {
        performBusinessLogic(message);
    } catch (std::exception& e) {
        // LOG(ERROR) << "call to business logic failed:";
        std::cout << "call to business logic failed:";
        // LOG(ERROR) << e.what();
    }
}

void storeParsedValues(json& text){
    std::vector<auto> keys;
    std::vector<auto> arrays;
    std::vector<auto> values;
    //this function will go through and store the values in correct spots
    //this is a template, as we need to test things still
    json::parser_callback_t cb = [](int depth, json::parse_event_t event, json& parsed){
        if(event == json::parse_event_t::key)
        {
            keys.push_back(event);
            return true;
        }
        else if(event == json::parse_event_t::array_end)
        {   
            arrays.push_back(event);
            return true;
        }
        else if(event == json::parse_event_t::value)
        {
            values.push_back(event);
            return true;
        }
        else
        {
            return false;
        }
        //this will create 3 large vectors
        //we need a way to store them in new vectors for each key value
        return;
    };
}