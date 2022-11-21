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
        throw recipientsWrapper{recipients, Response{Status::FAIL, "Player is not in any room"}};
    }

    auto roomCode = (*itr).second;
    return roomCode;
}

Room& Controller::findRoom(std::string roomCode) {
    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
        SPDLOG_ERROR("Could not find room with code: {}", roomCode);
        throw recipientsWrapper{recipients, Response{Status::FAIL, "Could not find room!"}};
    }

    Room& room = (*roomItr).second;
    return room;
}

Player& Controller::findPlayer(Room& room, networking::Connection& connectionInfo) {
    auto players = room.getAllPlayers();
    auto playerItr = std::find_if(players.begin(), players.end(), [&connectionInfo](const auto& player){
        return player.connectionID == connectionInfo.id;
    });
    if (playerItr == players.end()) {
        SPDLOG_ERROR("could not find player in room");
        throw recipientsWrapper{recipients, Response{Status::FAIL, "Could not find player!"}};
    }

    Player& player = *playerItr;
    return player;
}

void Controller::addPlayer(Room& room, std::string roomCode, networking::Connection& connectionInfo) {
    Player newPlayer {playerTypeEnum::player, connectionInfo};

    room::Response res = room.addPlayer(newPlayer);
    if (res.code == room::Status::Fail) {
        SPDLOG_ERROR("Connection:[{}] was unable to join room", connectionInfo.id);
        throw recipientsWrapper{recipients, Response{Status::FAIL, "Not allowed to join the room!"}};
    }

    PlayerLookUp.insert(std::pair{connectionInfo.id, roomCode});
}

void Controller::removePlayer(Room& room, networking::Connection& connectionInfo) {
    Player& player = findPlayer(room, connectionInfo);

    room::Response res = room.removePlayer(player);
    if (res.code == room::Status::Fail) {
        SPDLOG_ERROR("Player with connection: {}  failed to leave room", connectionInfo.id);
        throw recipientsWrapper{recipients, Response{Status::FAIL, "Failed to leave room"}};
    }

    PlayerLookUp.erase(connectionInfo.id);
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

    Room room = Room{configBuilder, newHost};
    GameRoomLookUp.insert(std::pair{newRandomCode, std::move(room)});
    PlayerLookUp.insert(std::pair{connectionInfo.id, newRandomCode});

    // 3. return the response
    SPDLOG_INFO("Room:{} has been created with host:[{}]", newRandomCode, connectionInfo.id);
    return recipientsWrapper{recipients,Response{Status::SUCCESS, newRandomCode}};
}

recipientsWrapper Controller::joinRoom(std::string roomCode, networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    SPDLOG_INFO("Player: {}  Attempting to join room", connectionInfo.id);
    try {
        Room& room = findRoom(roomCode);
        addPlayer(room, roomCode, connectionInfo);
        addToRecipients(room);
        SPDLOG_INFO("Connection:[{}] has joined room: {}", connectionInfo.id, roomCode);
    } catch (recipientsWrapper exception) {
        return exception;
    }
    
    return recipientsWrapper{recipients, Response{Status::SUCCESS, connectionInfo.id + " joined room"}};
}

recipientsWrapper Controller::leaveRoom(networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    SPDLOG_INFO("Player: {}  Attempting to leave room", connectionInfo.id);
    try {
        std::string roomCode = findRoomCode(connectionInfo);
        Room& room = findRoom(roomCode);
        removePlayer(room, connectionInfo);
        addToRecipients(room);
        SPDLOG_INFO("Player has left room: {}", roomCode);
    } catch (recipientsWrapper exception) {
        return exception;
    }

    return recipientsWrapper{recipients, Response{Status::SUCCESS, " left room"}};
}

recipientsWrapper Controller::startGame(networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    SPDLOG_INFO("Player: {}  Attempting to start game", connectionInfo.id);
    try {
        std::string roomCode = findRoomCode(connectionInfo);
        Room& room = findRoom(roomCode);
        Player& player = findPlayer(room, connectionInfo);

        room::Response res = room.startGame(player);
        if (res.code == room::Status::Fail) {
            throw recipientsWrapper{recipients, Response{Status::FAIL, "Failed to start game in room: " + roomCode}};
        }

        addToRecipients(room);
        SPDLOG_INFO("Starting game in room: {}", roomCode);
    } catch (recipientsWrapper exception) {
        return exception;
    }
    
    return recipientsWrapper{recipients, Response{Status::SUCCESS, "Game started"}};
}

recipientsWrapper Controller::endGame(networking::Connection& connectionInfo) {
    initRecipients();
    recipients.insert(connectionInfo);

    SPDLOG_INFO("Player: {}  Attempting to end game", connectionInfo.id);
    try {
        std::string roomCode = findRoomCode(connectionInfo);
        Room& room = findRoom(roomCode);
        Player& player = findPlayer(room, connectionInfo);

        room::Response res = room.endGame(player);
        if (res.code == room::Status::Fail) {
            throw recipientsWrapper{recipients, Response{Status::FAIL, "Failed to end game in room: " + roomCode}};
        }

        SPDLOG_INFO("Successfully ended game in room: {}", roomCode);
    } catch (recipientsWrapper exception) {
        return exception;
    }

    return recipientsWrapper{recipients, Response{Status::SUCCESS, "Game ended"}};
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
