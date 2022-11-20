#pragma once

#include "handler.h"
#include "Room.h"
#include "Player.h"
#include "Server.h"

#include <nlohmann/json.hpp>

#include <vector>
#include <map>
#include <string> 
#include <set>

enum Status {
    SUCCESS,
    FAIL
};
struct Response {
    Status status;
    std::string message;
};

struct recipientsWrapper{
    std::set<networking::Connection> recipientList;
    Response data;
    std::string actionName = "";
};


class Controller{

public:
    std::unordered_map<std::string, Room> GameRoomLookUp;
    // std::unordered_map<networking::Connection, int> PlayerLookUp;
    
    // create room from json file and host player connection
    //returns the random code to the room
    recipientsWrapper createRoom(json jsonFile, networking::Connection& connectionInfo);

    recipientsWrapper joinRoom(std::string roomCode, networking::Connection& connectionInfo);

    recipientsWrapper leaveRoom(std::string roomCode, networking::Connection& connectionInfo);

    recipientsWrapper startGame(std::string roomCode, networking::Connection& connectionInfo);

    recipientsWrapper endGame(networking::Connection& connectionInfo);

    recipientsWrapper handleUserInput(json userInput);

    
private:
    std::string generateRoomCode();
    std::set<networking::Connection> getConnections(Room& room);
    Room& findRoom(std::string roomCode, std::set<networking::Connection>& recipients);
    Player& findPlayer(std::string roomCode, networking::Connection& connectionInfo, std::set<networking::Connection>& recipients);
};