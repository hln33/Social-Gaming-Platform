#pragma once

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

    // std::find_if(playerLookup.begin(), playerLookup.end(), [connection& conectee](const auto& pair){
    //     return pair->second == conectee
    // })

    // if (it == playerLookup.end()) {

    // }
    
    // create room from json file and host player connection
    //returns the random code to the room
    Response createRoom(json jsonFile, networking::Connection connectionInfo);

    Response joinRoom(std::string roomCode, networking::Connection connectionInfo);

    Response leaveRoom(std::string roomCode, Player& playerInfo);

    Response handleUserInput(json userInput);

    
private:
    void deleteRoom(std::string roomCode) {
        //this->GameRoomLookUp.erase(roomCode);
    }
};