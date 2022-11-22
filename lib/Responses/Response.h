#include <string>


enum Status {
    Success,
    Fail,
};

struct Response {
    Status code;
    std::string message;
};

// Constant strings to be used in Response structs
// this is to avoid stringly typed API's
const std::string PLAYER_NOT_FOUND_IN_ROOM = "Player not found in room";
const std::string ROOM_NOT_FOUND = "Could not find roon";
const std::string REQUIREMENTS_NOT_MET = "Does not meet requirements";

namespace ResponseType {

const std::string CREATE_GAME_SUCCESS = "Game created";
const std::string LEFT_ROOM_SUCCESS = "Player left";
const std::string START_GAME_SUCCESS = "Game started";
const std::string END_GAME_SUCCESS = "Game ended";
const std::string JOIN_GAME_SUCCESS = "Player joined";

const std::string START_GAME_FAIL = "Game could not start";
const std::string END_GAME_FAIL = "Game could not end";
const std::string CREATE_GAME_FAIL = "Game could not be created";
const std::string LEFT_ROOM_FAIL = "Failed to leave game";
const std::string JOIN_GAME_FAIL = "Failed to join game";

const std::string ERROR = "Error";

}