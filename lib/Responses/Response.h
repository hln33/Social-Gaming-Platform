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

const std::string LEFT_ROOM_SUCCESS = "Player has left room";
const std::string START_GAME_SUCCESS = "Game started";
const std::string END_GAME_SUCCESS = "Game ended";

const std::string JOIN_GAME_FAIL = "Does not meet requirements";