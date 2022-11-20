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
    auto it = GameRoomLookUp.find(newRandomCode);

    while(it != GameRoomLookUp.end()){
        newRandomCode = randomCode();
    }

    return newRandomCode;
}

std::set<networking::Connection> Controller::getConnections(Room& room) {
    std::set<networking::Connection> recipients;

    auto players = room.getAllPlayers();
    for (auto& player : players) {
        networking::Connection connection;
        connection.id = player.connectionID;
        recipients.insert(connection);
    }

    return recipients;
}

Room& Controller::findRoom(std::string roomCode, std::set<networking::Connection>& recipients) {
    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
        SPDLOG_ERROR("Could not find room with code: {}", roomCode);
        throw recipientsWrapper{recipients, Response{Status::FAIL, "Could not find room!"}};
    }

    Room& room = (*roomItr).second;
    return room;
}

Player& Controller::findPlayer(std::string roomCode, networking::Connection& connectionInfo, std::set<networking::Connection>& recipients) {
    Room& room = findRoom(roomCode, recipients);
    auto players = room.getAllPlayers();
    auto playerItr = std::find_if(players.begin(), players.end(), [&connectionInfo](const auto& player){
        return player.connectionID == connectionInfo.id;
    });
    if (playerItr == players.end()) {
        SPDLOG_ERROR("could not find player in room with code: {}", roomCode);
        throw recipientsWrapper{recipients, Response{Status::FAIL, "Could not find player!"}};
    }

    Player& player = *playerItr;
    return player;
}

void Controller::addPlayer(std::string roomCode, networking::Connection& connectionInfo, std::set<networking::Connection>& recipients) {
    Room& room = findRoom(roomCode, recipients);
    Player newPlayer {playerTypeEnum::player, connectionInfo};

    Room::Response res = room.addPlayer(newPlayer);
    if (res.status.statusCode == Room::Status::Fail) {
        SPDLOG_ERROR("Connection:[{}] was unable to join room:{}", connectionInfo.id, roomCode);
        throw recipientsWrapper{recipients, Response{Status::FAIL, "Not allowed to join the room!"}};
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC
////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: might not need the client to store the player id because we can use the connection info
// instead to uniquely determine which player


recipientsWrapper Controller::createRoom(json jsonFile, networking::Connection& connectionInfo) {    
    //make host and add them to the player hashtable
    Player newHost = Player{playerTypeEnum::host, connectionInfo};
    // PlayerLookUp.insert(std::pair(connectionInfo, newHost.getId()));
    
    // 1. generate random code
    std::string newRandomCode = generateRoomCode();

    // 2. add the room into GameRoomLookup
    RoomConfigBuilderOptions configBuilder = extractConfig(jsonFile);

    Room room = Room{configBuilder, newHost};
    GameRoomLookUp.insert(std::pair<std::string, Room>(newRandomCode, std::move(room)));

    // 3. return the response
    std::set<networking::Connection> recipients;
    recipients.insert(connectionInfo);

    // recipientsWrapper
    SPDLOG_INFO("Room:{} has been created with host:[{}]", newRandomCode, connectionInfo.id);
    return recipientsWrapper{recipients,Response{Status::SUCCESS, newRandomCode}};
}

recipientsWrapper Controller::joinRoom(std::string roomCode, networking::Connection& connectionInfo) {
    std::set<networking::Connection> recipients;
    recipients.insert(connectionInfo);

    try {
        Room& room = findRoom(roomCode, recipients);
        addPlayer(roomCode, connectionInfo, recipients);

        auto players = getConnections(room);
        recipients.insert(players.begin(), players.end());
    } catch (recipientsWrapper exception) {
        return exception;
    }
    
    SPDLOG_INFO("Connection:[{}] has joined room:{}", connectionInfo.id, roomCode);
    return recipientsWrapper{recipients, Response{Status::SUCCESS, connectionInfo.id + " joined room"}};
}

recipientsWrapper Controller::leaveRoom(std::string roomCode, networking::Connection& connectionInfo) {
    std::set<networking::Connection> recipients;
    recipients.insert(connectionInfo);

    try {
        Room& room = findRoom(roomCode, recipients);
        Player& player = findPlayer(roomCode, connectionInfo, recipients);

        room.removePlayer(player);
        auto players = getConnections(room);
        recipients.insert(players.begin(), players.end());
    } catch (recipientsWrapper exception) {
        return exception;
    }

    SPDLOG_INFO("Connection:[{}] has left room: {}", connectionInfo.id, roomCode);
    return recipientsWrapper{recipients, Response{Status::SUCCESS, " left room"}};
}

recipientsWrapper Controller::startGame(std::string roomCode, networking::Connection& connectionInfo) {
    std::set<networking::Connection> recipients;
    recipients.insert(connectionInfo);

    try {
        Room& room = findRoom(roomCode, recipients);
        Player& player = findPlayer(roomCode, connectionInfo, recipients);
        room.startGame(player);
    } catch (recipientsWrapper exception) {
        return exception;
    }
    
    SPDLOG_INFO("Starting game in room: {}", roomCode);
    return recipientsWrapper{recipients, Response{Status::SUCCESS, "Game started"}};
}

// recipientsWrapper endGame(networking::Connection& connectionInfo) {

// }


// recipientsWrapper Controller::handleUserInput(json userInput) {
//     // send userInput to room that player is in
//     // json data to get room code
//     // use roomCode to lookup room
//     // 

//     //int roomCode = userInput["room_code"];
//     //userInput["Player_info"];
//     //userInput["data"];
// }
