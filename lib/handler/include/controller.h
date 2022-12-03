#pragma once

#include "handler.h"
#include "Room.h"
#include "Player.h"
#include "Server.h"

#include <nlohmann/json.hpp>

#include <map>
#include <string> 
#include <set>
 

struct recipientsWrapper{
    std::set<networking::Connection> recipientList;
    Response data;
    std::string responseCode;
};


class Controller{

public:
    std::unordered_map<std::string, Room> GameRoomLookUp;
    std::unordered_map<uintptr_t, std::string> PlayerLookUp;
    
    // create room from json file and host player connection
    //returns the random code to the room
    recipientsWrapper createRoom(json jsonFile, networking::Connection& connectionInfo);

    recipientsWrapper joinRoom(std::string roomCode, networking::Connection& connectionInfo);

    recipientsWrapper leaveRoom(networking::Connection& connectionInfo);

    recipientsWrapper startGame(networking::Connection& connectionInfo);

    recipientsWrapper endGame(networking::Connection& connectionInfo);

    recipientsWrapper handleUserInput(json userInput);

    
private:
    std::set<networking::Connection> recipients;

    std::string generateRoomCode();
    void addToRecipients(Room& room);
    void initRecipients();

    std::string findRoomCode(networking::Connection connectionInfo);
    Room& findRoom(std::string roomCode);
};