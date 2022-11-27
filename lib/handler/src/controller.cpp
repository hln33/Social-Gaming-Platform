// Desription 

#include "RoomConfig.h"
#include "controller.h"
#include "handlerHelper.h"

#include <spdlog/spdlog.h>

////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////

std::string Controller::generateRoomCode() {
    std::string newRandomCode = randomCode();
    while (GameRoomLookUp.find(newRandomCode) != GameRoomLookUp.end()) {
        newRandomCode = randomCode();
    }

    return newRandomCode;
}

void Controller::addToRecipients(Room& room) {
    auto players = room.getAllPlayers();
    for (auto& player : players) {
        networking::Connection connection;
        connection.id = player.connectionID;
        recipients.insert(connection);
    }
}

void Controller::initRecipients() {
    recipients.clear();
}

std::string Controller::findRoomCode(networking::Connection connectionInfo) {
    auto connectionID = connectionInfo.id;
    auto itr = PlayerLookUp.find(connectionID);
    if (itr == PlayerLookUp.end()) {
        SPDLOG_ERROR("Player does not belong in a room");
        throw Response{Status::Fail, "Player is not in any room"};
    }

    auto roomCode = (*itr).second;
    return roomCode;
}

Room& Controller::findRoom(std::string roomCode) {
    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
        SPDLOG_ERROR("Could not find room with code: {}", roomCode);
        throw Response{Status::Fail, "Could not find room!"};
    }

    Room& room = (*roomItr).second;
    return room;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC
////////////////////////////////////////////////////////////////////////////////////////////////

recipientsWrapper Controller::createRoom(json jsonFile, networking::Connection& connectionInfo) {    
    initRecipients();
    recipients.insert(connectionInfo);

    //make host and add them to the player hashtable
    Player newHost = Player{playerTypeEnum::host, connectionInfo};
    // PlayerLookUp.insert(std::pair(connectionInfo, newHost.getId()));
    
    // 1. generate random code
    std::string newRandomCode = generateRoomCode();
    
    // 2. add the room into GameRoomLookup
    RoomConfigBuilderOptions configBuilder = extractConfig(jsonFile);
    getRuleTree(jsonFile);
    Room room = Room{configBuilder, newHost};
    GameRoomLookUp.insert(std::pair{newRandomCode, std::move(room)});
    PlayerLookUp.insert(std::pair{connectionInfo.id, newRandomCode});

    // 3. return the response
    SPDLOG_INFO("Room:{} has been created with host:[{}]", newRandomCode, connectionInfo.id);
    return recipientsWrapper{recipients,Response{Status::Success, newRandomCode}};
}

recipientsWrapper Controller::joinRoom(std::string roomCode, networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    SPDLOG_INFO("Player:{}  Attempting to join room", connectionInfo.id);
    try {
        Room& room = findRoom(roomCode);
        Player newPlayer {playerTypeEnum::player, connectionInfo};
        Response res = room.addPlayer(newPlayer);
        addToRecipients(room);
    } catch (Response exception) {
        return recipientsWrapper{recipients, Response{exception.code, exception.message}};
    }
    PlayerLookUp.insert(std::pair{connectionInfo.id, roomCode});

    SPDLOG_INFO("Player:{} has joined room:{}", connectionInfo.id, roomCode);
    return recipientsWrapper{recipients, Response{Status::Success, connectionInfo.id + " joined room"}};
}

recipientsWrapper Controller::leaveRoom(networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    std::string roomCode = findRoomCode(connectionInfo);
    Room& room = findRoom(roomCode);
    Player player = room.findPlayer(connectionInfo.id);

    SPDLOG_INFO("Player:{} attempting to leave room:{}", connectionInfo.id, roomCode);
    try {
        Response res = room.removePlayer(player);
        addToRecipients(room);
    } catch (Response exception) {
        SPDLOG_ERROR("Player:{} could not leave room:{}", connectionInfo.id, roomCode);
        return recipientsWrapper{recipients, Response{exception.code, exception.message}};
    }
    PlayerLookUp.erase(connectionInfo.id);

    SPDLOG_INFO("Player:{} has left room:{}", connectionInfo.id, roomCode);
    return recipientsWrapper{recipients, Response{Status::Success, " left room"}};
}

recipientsWrapper Controller::startGame(networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    std::string roomCode = findRoomCode(connectionInfo);
    Room& room = findRoom(roomCode);
    Player player = room.findPlayer(connectionInfo.id);

    SPDLOG_INFO("Player:{}  Attempting to start game in room:{}", connectionInfo.id, roomCode);
    try {
        Response res = room.startGame(player);
        addToRecipients(room);
    } catch (Response exception) {
        SPDLOG_ERROR("Could not start game in room:{}", roomCode);
        return recipientsWrapper{recipients, Response{exception.code, exception.message}};
    } 
    
    SPDLOG_INFO("Starting game in room:{}", roomCode);
    return recipientsWrapper{recipients, Response{Status::Success, "Game started"}};
}

recipientsWrapper Controller::endGame(networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    std::string roomCode = findRoomCode(connectionInfo);
    Room& room = findRoom(roomCode);
    Player player = room.findPlayer(connectionInfo.id);

    SPDLOG_INFO("Player:{}  Attempting to end game", connectionInfo.id);
    try {
        Response res = room.endGame(player);
    } catch (Response exception) {
        SPDLOG_ERROR("Could not end game in room:{}", roomCode);
        return recipientsWrapper{recipients, Response{exception.code, exception.message}};
    } 

    SPDLOG_INFO("Successfully ended game in room: {}", roomCode);
    return recipientsWrapper{recipients, Response{Status::Success, "Game ended"}};
}


// recipientsWrapper Controller::handleUserInput(json userInput) {
//     // send userInput to room that player is in
//     // json data to get room code
//     // use roomCode to lookup room
//     // 

//     //int roomCode = userInput["room_code"];
//     //userInput["Player_info"];
//     //userInput["data"];
// }
