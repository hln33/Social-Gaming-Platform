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
        SPDLOG_ERROR("Player:{} does not belong in a room", connectionID);
        throw Response{PLAYER_NOT_FOUND_IN_ROOM};
    }

    auto roomCode = (*itr).second;
    return roomCode;
}

Room& Controller::findRoom(std::string roomCode) {
    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
        SPDLOG_ERROR("Could not find room with code:{}", roomCode);
        throw Response{ROOM_NOT_FOUND};
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
    
    // 1. generate random code
    std::string newRandomCode = generateRoomCode();

    // 2. add the room into GameRoomLookup
    RoomConfigBuilderOptions configBuilder = extractConfig(jsonFile);
    Player newHost = Player{playerTypeEnum::host, connectionInfo};
    Room room = Room{configBuilder, newHost};
    
    GameRoomLookUp.insert(std::pair{newRandomCode, std::move(room)});
    PlayerLookUp.insert(std::pair{connectionInfo.id, newRandomCode});

    // 3. return the response
    SPDLOG_INFO("Room:{} has been created with host:{}", newRandomCode, connectionInfo.id);
    return recipientsWrapper{recipients, Response{newRandomCode}, ResponseCode::CREATE_GAME_SUCCESS};
}

recipientsWrapper Controller::joinRoom(std::string roomCode, networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    SPDLOG_INFO("Player:{}  Attempting to join room:{}", connectionInfo.id, roomCode);
    try {
        Room& room = findRoom(roomCode);
        Player newPlayer {playerTypeEnum::player, connectionInfo};
        room.addPlayer(newPlayer);
        addToRecipients(room);
    } catch (Response exception) {
        SPDLOG_ERROR("Player:{} Unable to join room:{}", connectionInfo.id, roomCode);
        return recipientsWrapper{recipients, exception, ResponseCode::JOIN_GAME_FAIL};
    }

    SPDLOG_INFO("Connection:{} has joined room:{}", connectionInfo.id, roomCode);
    PlayerLookUp.insert(std::pair{connectionInfo.id, roomCode});
    return recipientsWrapper{recipients, Response{std::to_string(connectionInfo.id)}, ResponseCode::JOIN_GAME_SUCCESS};
}

recipientsWrapper Controller::leaveRoom(networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    // don't need to check for exceptions because player will not have option to leave room unless they are already in one
    std::string roomCode = findRoomCode(connectionInfo);
    Room& room = findRoom(roomCode);
    Player player = room.findPlayer(connectionInfo.id);

    SPDLOG_INFO("Player:{}  Attempting to leave room", connectionInfo.id);
    try {
        room.removePlayer(player);
        addToRecipients(room);
    } catch (Response exception) {
        return recipientsWrapper{recipients, exception, ResponseCode::LEFT_ROOM_FAIL};
    }

    SPDLOG_INFO("Player has left room:{}", roomCode);
    PlayerLookUp.erase(connectionInfo.id);
    return recipientsWrapper{recipients, Response{NO_DATA}, ResponseCode::LEFT_ROOM_SUCCESS};
}

recipientsWrapper Controller::startGame(networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    // don't need to check for exceptions because player will not have option to start game unless they are already in one
    std::string roomCode = findRoomCode(connectionInfo);
    Room& room = findRoom(roomCode);
    Player player = room.findPlayer(connectionInfo.id);

    SPDLOG_INFO("Player:{}  Attempting to start game in room:{}", connectionInfo.id, roomCode);
    try {
        Response res = room.startGame(player);
        addToRecipients(room);
    } catch (Response exception) {
        SPDLOG_ERROR("Could not start game in room:{}", roomCode);
        return recipientsWrapper{recipients, exception, ResponseCode::START_GAME_FAIL};
    } 
    
    SPDLOG_INFO("Starting game in room:{}", roomCode);
    return recipientsWrapper{recipients, Response{NO_DATA}, ResponseCode::START_GAME_SUCCESS};
}

recipientsWrapper Controller::endGame(networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    // don't need to check for exceptions because player will not have option to start game unless they are already in one
    std::string roomCode = findRoomCode(connectionInfo);
    Room& room = findRoom(roomCode);
    Player player = room.findPlayer(connectionInfo.id);

    SPDLOG_INFO("Player:{}  Attempting to end game in room:{}", connectionInfo.id, roomCode);
    try {
        Response res = room.endGame(player);
    } catch (Response exception) {
        SPDLOG_ERROR("Player:{} Could not end game in room:{}", connectionInfo.id, roomCode);
        return recipientsWrapper{recipients, exception, ResponseCode::END_GAME_FAIL};
    } 

    SPDLOG_INFO("Successfully ended game in room:{}", roomCode);
    return recipientsWrapper{recipients, Response{NO_DATA}, ResponseCode::END_GAME_SUCCESS};
}


recipientsWrapper Controller::handleUserInput(networking::Connection& connectionInfo, json userInput) {
    initRecipients();
    recipients.insert(connectionInfo);

    std::string roomCode = findRoomCode(connectionInfo);
    Room& room = findRoom(roomCode);

    try {
        Response res = room.inputChoice(userInput);
        return recipientsWrapper{recipients, res, ResponseCode::GAME_INPUT_SUCCESS};
    } catch (Response exception) {
        return recipientsWrapper{recipients, exception, ResponseCode::GAME_INPUT_FAIL};
    }
}
