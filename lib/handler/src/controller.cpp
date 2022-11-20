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
    // 1. lookup the room in GameRoomLookup
    // 1.1 if could not find room send error response
    // 1.2 otherwise ask the room if player can join
    // 1.2.1 if no then send the response
    // 1.2.2 if yes then send the response

    std::set<networking::Connection> recipients;
    recipients.insert(connectionInfo);

    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
        SPDLOG_ERROR("Could not find room with code: {}", roomCode);
        return recipientsWrapper{recipients, Response{Status::FAIL, "Could not find room!"}};
    }
    
    Room& room = (*roomItr).second;
    Player newPlayer {playerTypeEnum::player, connectionInfo};
    Room::Response res = room.addPlayer(newPlayer);
    if (res.status.statusCode == Room::Status::Fail) {
        SPDLOG_ERROR("Connection:[{}] was unable to join room:{}", connectionInfo.id, roomCode);
        return recipientsWrapper{recipients, Response{Status::FAIL, "Not allowed to join the room!"}};
    }
    auto players = getConnections(room);
    recipients.insert(players.begin(), players.end());

    SPDLOG_INFO("Connection:[{}] has joined room:{}", connectionInfo.id, roomCode);
    return recipientsWrapper{recipients, Response{Status::SUCCESS, newPlayer.getName() + " joined room"}};
}

recipientsWrapper Controller::leaveRoom(std::string roomCode, networking::Connection& connectionInfo) {

    std::set<networking::Connection> recipients;
    recipients.insert(connectionInfo);

    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
        SPDLOG_ERROR("Could not find room with code: {}", roomCode);
        return recipientsWrapper{recipients, Response{Status::FAIL, "Could not find room!"}};
    }
    // auto playerItr = PlayerLookUp.find(connectionInfo);
    // if (playerItr == PlayerLookUp.end()) {
    //     recipients.emplace_back(connectionInfo);
    //     return recipientsWrapper{recipients, Response{Status::FAIL, "Could not find player!"}};
    // }

    Room& room = (*roomItr).second; 
    // int playerId = (*playerItr).second;
    //room.removePlayer(playerId); I think we may have to change room.removePlayer to take in playerID instead of player object


    auto players = getConnections(room);
    recipients.insert(players.begin(), players.end());

    SPDLOG_INFO("Connection:[{}] has left room: {}", connectionInfo.id, roomCode);
    return recipientsWrapper{recipients, Response{Status::SUCCESS, " left room"}};

    //this one later to show who left room
    // return recipientsWrapper{recipients, Response{Status::SUCCESS, player.getName() + " left room"}};
}

recipientsWrapper Controller::startGame(networking::Connection& connectionInfo) {

}

recipientsWrapper endGame(networking::Connection& connectionInfo) {
    
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
