#include <iostream>
#include "handler.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
// #include "../../gameLogic/business_logic_interface.h"


using json = nlohmann::json;

void initLogging() {
    //google::InitGoogleLogging("Handler");
}

// checks if a given string is valid JSON
bool isJSON(const std::string& text) {
    return json::accept(text);
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
    };
}


bool MessageContains(const std::string& string, const std::string& subString) {
    return string.find(subString) != std::string::npos;
}

// void handleConfig(const json j_complete){

//     std::string roomName = j_complete["configuration"]["name"];
//     int minPlayers = j_complete["configuration"]["player count"]["min"];
//     int maxPlayers = j_complete["configuration"]["player count"]["max"];

//     /* print room info */
//     std::cout << "Print room info: "<< "\n";
//     std::cout << "RoomName: " << roomName << "\n";
//     std::cout << "Min, max players : " << minPlayers << ", " << maxPlayers << "\n";

//     // calling business interface from here, wait for room.h to be fixed 
//     // IRoomConfig config_obj = createRoomConfigBuilder()
//     // IRoom room = createRoom(config_obj)
// }

// void handleConstant(const json j_complete){
//     // example of extracting the first key ("weapon")
//     std::string first_key = j_complete["constants"].items().begin().key();
//     std::vector<std::pair<std::string, std::string>> beat_rules;
//     // example of how to extract weapon elements with a vector  
//     std::cout << "\n" << "Print constant info: " << "\n";
//     std::vector<json> weapon_json = j_complete["constants"].items().begin().value();
//     for (auto j : weapon_json){
//         auto first_key = j.items().begin(); // name rock
//         auto second_key = ++j.items().begin(); // beat scissors
//         std::cout << second_key.key() << " | " << second_key.value() << "\n"; 
//         std::cout << first_key.key() << " | " << first_key.value() << "\n";
//         std::pair<std::string, std::string> beat = {second_key.value(), first_key.value()};
//         beat_rules.push_back(beat);
//     }
//     // return beat_rules;
// }

// void handleRules(const json j_complete){
//     if (isArray(j_complete["rules"])){
//         json j_rule = j_complete["rules"];
//         // extract all rules into vector
//         std::vector<json> individual_rules = j_rule;
        
//         // extract individual rule
//         for (auto r : individual_rules){
//             std::string rule_name = "empty";
//             std::string rule_element = "empty";
//             std::string rule_list = "empty";
//             std::string rule_value = "empty";
//             if (r.contains("rule")){
//                 rule_name = r["rule"];
//             }
//             if (r.contains("element")){
//                 rule_element = r["element"];
//             }
//             if (r.contains("list")){
//                 rule_list = r["list"];
//             }
//             if (r.contains("value")){
//                 rule_value = r["value"];
//             }
//             // wait for business interface for create rule objects 
//         }
//     }
//     else {
//         // std::cout << "rule is not array" << "\n";
//     }
// }

// // comments below contain pseudo-code
// void performBusinessLogic(const std::string& message) {
    
//     if (!json::accept(message)){
//         // LOG(ERROR) << "sent message is not JSON";
//         throw std::invalid_argument("sent message is not JSON");
//     }

//     json j_complete = completeParse(message);
//     if (j_complete.contains("configuration")){
//         handleConfig(j_complete);
//     }
//     if (j_complete.contains("constants")){
//         handleConstant(j_complete);
//     }
//     if (j_complete.contains("rules")){
//         handleRules(j_complete);
//     }

    
//     if (MessageContains(message, "configuration")){
//         // LOG(INFO) << "A new room has been created";
//     }
//     else if (MessageContains(message, "Player Left")) {
//         // std::string playerName = getPlayerName(message);
//         // businesslogic::removePlayer(playerName);
//         // LOG(INFO) << "A player has left";
//         std::cout << "A player has left";
//     } else if (MessageContains(message, "Player Joined")) {
//         // std::string playerName = getPlayerName(message);
//         // businesslogic::addPlayer(playerName);
//         // LOG(INFO) << "A player has joined";
//         std::cout << "A player has joined";
//     } else if (MessageContains(message, "Game Ended")) {
//         // businesslogic::endGame();
//         // LOG(INFO) << "A game has ended";
//         std::cout << "A game has ended";
//     } else if (MessageContains(message, "Game Created")) {
//         // Configuration config = parseJSON(message);
//         // businesslogic::createGame(config);
//         // LOG(INFO) << "A game has been created";
//         std::cout << "A game has been created";
//     } else {
//         // LOG(ERROR) << "invalid message";
//         std::cout << "invalid message";
//         throw std::runtime_error("invalid message passed to handler");
//     }
// }

// recieves message from networking
// For now, input will be assumed to be a string
bool recieveMessage(std::string& message) {
    // google::InitGoogleLogging("Handler");

    try {
        performBusinessLogic(message);
        return true;
    } catch (std::exception& e) {
        // LOG(ERROR) << "call to business logic failed:";
        std::cout << "call to business logic failed:";
        return false;
        // LOG(ERROR) << e.what();
    }
}


// bool storeParsedValues(json& text){
//     //this function will go through and store the values in correct spots
//     //this is a template, as we need to test things still
//     json::parser_callback_t cb = [](int depth, json::parse_event_t event, json& parsed){
//         std::vector<json> keys;
//         std::vector<json> arrays;
//         std::vector<json> values;
//         if(event == json::parse_event_t::key)
//         {
//             keys.push_back(event);
//             return true;
//         }
//         else if(event == json::parse_event_t::array_end)
//         {   
//             arrays.push_back(event);
//             return true;
//         }
//         else if(event == json::parse_event_t::value)
//         {
//             values.push_back(event);
//             return true;
//         }
//         else
//         {
//             return false;
//         }
//         //this will create 3 large vectors
//         //we need a way to store them in new vectors for each key value
//         return true;
//     };
// }



// this is the original storedParsedValue() but modified last else statement 
// to return true to fix a bug that callback cannot detect value event and array event
bool storeParsedValuesRevised(std::string& message){
    std::vector<json> keys;
    std::vector<json> arrays;
    std::vector<json> values;
    if (!json::accept(message))
        return false;
    json j_complete = completeParse(message);

    // define parser callback
    json::parser_callback_t cb = [&keys, &arrays, &values](int depth, json::parse_event_t event, json & parsed)
    {
        if (event == json::parse_event_t::key)
        {
            keys.push_back(parsed);
            std::cout << "Key Event: "<< parsed << "\n";
            return true;
        }
        else if(event == json::parse_event_t::value)
        {
            values.push_back(parsed);
            std::cout << "Value event:" << parsed << "\n";
            
            return true;
        }
        else if(event == json::parse_event_t::array_end)
        {   
            arrays.push_back(parsed);
            std::cout << "Array event:" << parsed << "\n";
            return true;
        }
        else
        {
            return true;
        }
    };

    // parse (with callback) and serialize JSON
    json j_filtered = json::parse(message, cb);
    return true;
}