#pragma once

#include "handler.h"
#include "Room.h"
#include "Player.h"
#include <map>
#include <string> 
#include "Server.h"
#include <nlohmann/json.hpp>
#include <vector>

enum Status {
    SUCCESS,
    FAIL
};
struct Response {
    Status status;
    std::string message;
};

struct recipientsWrapper{
    std::vector<networking::Connection> recipientList;
    Response data;
    std::string actionName = "";
};


class Controller{

public:
    std::unordered_map<std::string, Room> GameRoomLookUp;
    // std::unordered_map<networking::Connection, int> PlayerLookUp;
    
    // create room from json file and host player connection
    //returns the random code to the room
    recipientsWrapper createRoom(json jsonFile, networking::Connection connectionInfo);

    recipientsWrapper joinRoom(std::string roomCode, networking::Connection connectionInfo);

    recipientsWrapper leaveRoom(std::string roomCode, networking::Connection connectionInfo);

    recipientsWrapper handleUserInput(json userInput);

    
private:
    std::string generateRoomCode();
    Room createRoomFromConfig(json jsonFile, Player host);
};