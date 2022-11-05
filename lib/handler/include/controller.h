
#include "handler.h"
#include "Room.h"
#include "Player.h"
#include <map>
#include <string> 
#include "Server.h"
#include <nlohmann/json.hpp>

enum Status {
    SUCCESS,
    FAIL
};

struct Response {
    Status status;
    std::string message;
};

class Controller{

public:

    // std::unordered_map<std::string, int> randomCodeLookUp;// maps the random room code to room code -> 
                                                          // might not need this
    std::unordered_map<std::string, Room> GameRoomLookUp;
    std::unordered_map<int, networking::Connection> playerLookUp;
    
    // create room from json file and host player connection
    //returns the random code to the room
    Response createRoom(json jsonFile, networking::Connection player);
    

    RoomConfigInterface createRoomConfig(json jsonFile);


    Response joinRoom(int roomCode, PlayerInterface& playerInfo);

    Response leaveRoom(int roomCode, PlayerInterface& playerInfo);

    Response handleUserInput(json userInput);

    
private:
    void Controller::deleteRoom(std::string roomCode) {
        this->GameRoomLookUp.erase(roomCode);
    }
};