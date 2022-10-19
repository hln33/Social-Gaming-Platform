#include "handler.h"
#include <glog/logging.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
// #include "../../gameLogic/business_logic_interface.h"

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
    // this function can most likely just call https://json.nlohmann.me/api/basic_json/accept/. Just currently waiting on Bikram to integrate the library into the project
    // something like this:
    // return nlohmann::json::accept(text)
    return json::accept(text);
    // return false; placeholder return statement
}

bool isString(const json j_value){
    return (j_value.is_string());
}

bool isInt(const json j_value){
    return (j_value.is_number_integer());
}

bool isArray(const json j_value){
    return (j_value.is_array());
}
bool isBoolean(const json j_value){
    return (j_value.is_boolean());
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
    }
    //refer to json.nlohmann.me/api/basic_json/parse/ for breakdown of parse
    json::parser_callback_t cb = [](int depth, json::parse_event_t event, json& parsed){
        //this can be used to sort through the parser elements after reading
        return NULL;
    };
}


bool MessageContains(const std::string& string, const std::string& subString) {
    return string.find(subString) != std::string::npos;
}

void handleConfig(const json j_complete){

    std::string roomName = j_complete["configuration"]["name"];
    int minPlayers = j_complete["configuration"]["player count"]["min"];
    int maxPlayers = j_complete["configuration"]["player count"]["max"];

    /* print room info */
    std::cout << "Print room info: "<< "\n";
    std::cout << "RoomName: " << roomName << "\n";
    std::cout << "Min, max players : " << minPlayers << ", " << maxPlayers << "\n";

    // calling business interface from here, wait for room.h to be fixed 
    // IRoomConfig config_obj = createRoomConfigBuilder()
    // IRoom room = createRoom(config_obj)
}

void handleConstant(const json j_complete){
    // example of extracting the first key ("weapon")
    std::string first_key = j_complete["constants"].items().begin().key();
    std::vector<std::pair<std::string, std::string>> beat_rules;
    // example of how to extract weapon elements with a vector  
    std::cout << "\n" << "Print constant info: " << "\n";
    std::vector<json> weapon_json = j_complete["constants"].items().begin().value();
    for (auto j : weapon_json){
        auto first_key = j.items().begin(); // name rock
        auto second_key = ++j.items().begin(); // beat scissors
        std::cout << second_key.key() << " | " << second_key.value() << "\n"; 
        std::cout << first_key.key() << " | " << first_key.value() << "\n";
        std::pair<std::string, std::string> beat = {second_key.value(), first_key.value()};
        beat_rules.push_back(beat);
    }
    // return beat_rules;
}

void handleRules(const json j_complete){
    if (isArray(j_complete["rules"])){
        json j_rule = j_complete["rules"];
        // extract all rules into vector
        std::vector<json> individual_rules = j_rule;
        
        // extract individual rule
        for (auto r : individual_rules){
            std::string rule_name = "empty";
            std::string rule_element = "empty";
            std::string rule_list = "empty";
            std::string rule_value = "empty";
            if (r.contains("rule")){
                rule_name = r["rule"];
            }
            if (r.contains("element")){
                rule_element = r["element"];
            }
            if (r.contains("list")){
                rule_list = r["list"];
            }
            if (r.contains("value")){
                rule_value = r["value"];
            }
            // wait for business interface for create rule objects 
        }
    }
    else {
        // std::cout << "rule is not array" << "\n";
    }
}

// comments below contain pseudo-code
void performBusinessLogic(const std::string& message) {
    
    if (!isJSON(message)){
        LOG(ERROR) << "sent message is not JSON";
        throw std::invalid_argument("sent message is not JSON");
    }

    json j_complete = completeParse(message);
    if (j_complete.contains("configuration")){
        handleConfig(j_complete);
    }
    if (j_complete.contains("constants")){
        handleConstant(j_complete);
    }
    if (j_complete.contains("rules")){
        handleRules(j_complete);
    }


    if (MessageContains(message, "configuration")){
        LOG(INFO) << "A new room has been created";
    }
    else if (MessageContains(message, "Player Left")) {
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