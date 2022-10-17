#include "../include/handler.h"

// checks if a given string is valid JSON

bool isJSON(const std::string& text) {
    // this function can most likely just call https://json.nlohmann.me/api/basic_json/accept/. Just currently waiting on Bikram to integrate the library into the project
    // something like this:
    // return nlohmann::json::accept(text)
    return json::accept(text);
    // return false; placeholder return statement
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

// comments below contain pseudo-code
void performBusinessLogic(const std::string& message, GameLogic& gamelogic) {
    // new section 
    json j_complete = completeParse(message);
    if (j_complete.contains("configuration")){
        std::string roomName = j_complete["configuration"]["name"];
        std::string playerMin = j_complete["configuration"]["player count"]["min"];
        std::string playerMax = j_complete["configuration"]["player count"]["max"];
        RoomConfig rc(roomName, stoi(playerMin), stoi(playerMax));
        PlayerStorage ps;
        Room r  = {rc , ps};
        gamelogic.addRooms(r);
    }

    
    
// previous section
//     if (MessageContains(message, "Player Left")) {
//         // std::string playerName = getPlayerName(message);
//         // businesslogic::removePlayer(playerName);
//         LOG(INFO) << "A player has left";
//     } else if (MessageContains(message, "Player Joined")) {
//         // std::string playerName = getPlayerName(message);
//         // businesslogic::addPlayer(playerName);
//         LOG(INFO) << "A player has joined";
//     } else if (MessageContains(message, "Game Ended")) {
//         // businesslogic::endGame();
//         LOG(INFO) << "A game has ended";
//     } else if (MessageContains(message, "Game Created")) {
//         // Configuration config = parseJSON(message);
//         // businesslogic::createGame(config);
//         LOG(INFO) << "A game has been created";
//     } else {
//         LOG(ERROR) << "invalid message";
//         throw std::runtime_error("invalid message passed to handler");
//     }
// }

// recieves message from networking
// For now, input will be assumed to be a string
}
void recieveMessage(std::string& message, GameLogic& gamelogic) {
    google::InitGoogleLogging("Handler");

    try {
        performBusinessLogic(message, gamelogic);
    } catch (std::exception& e) {
        LOG(ERROR) << "call to business logic failed:";
        LOG(ERROR) << e.what();
    }
}

