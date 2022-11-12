// Desription 

#include "RoomConfig.h"
#include "controller.h"
#include "handlerHelper.h"

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

////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC
////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: might not need the client to store the player id because we can use the connection info
// instead to uniquely determine which player


recipientsWrapper Controller::createRoom(json jsonFile, networking::Connection connectionInfo) {    
    //make host and add them to the player hashtable
    Player newHost = Player{playerTypeEnum::host};
    newHost.setConnection(connectionInfo);
    // PlayerLookUp.insert(std::pair(connectionInfo, newHost.getId()));
    
    // 1. generate random code
    std::string newRandomCode = generateRoomCode();

    // 2. add the room into GameRoomLookup
    RoomConfigBuilderOptions configBuilder = extractConfig(jsonFile);

    Room room = Room{configBuilder, newHost};
    GameRoomLookUp.insert(std::pair<std::string, Room>(newRandomCode, std::move(room)));
     // // 3. return the response

    std::vector<networking::Connection> recipients;
    recipients.emplace_back(connectionInfo);

    // recipientsWrapper
    return recipientsWrapper{recipients,Response{Status::SUCCESS, newRandomCode}};
}

recipientsWrapper Controller::joinRoom(std::string roomCode, networking::Connection connectionInfo) {
    // 1. lookup the room in GameRoomLookup
    // 1.1 if could not find room send error response
    // 1.2 otherwise ask the room if player can join
    // 1.2.1 if no then send the response
    // 1.2.2 if yes then send the response

    
    
    std::vector<networking::Connection> recipients;

    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
        recipients.emplace_back(connectionInfo);
        return recipientsWrapper{recipients, Response{Status::FAIL, "Could not find room!"}};
    }
    
    Room& room = (*roomItr).second;
    Player newPlayer {playerTypeEnum::player};
    newPlayer.setConnection(connectionInfo);
    Room::Response res = room.addPlayer(newPlayer);
    if (res.status.statusCode == Room::Status::Fail) {
        recipients.emplace_back(connectionInfo);
        return recipientsWrapper{recipients, Response{Status::FAIL, "Not allowed to join the room!"}};
    }
    
   
    // loop through the players to send a message to
    auto players = room.getAllPlayers();
    for (auto player : players) {
        recipients.emplace_back(player.getConnection());
    }

    return recipientsWrapper{recipients, Response{Status::SUCCESS, newPlayer.getName() + " joined room"}};
}

recipientsWrapper Controller::leaveRoom(std::string roomCode, networking::Connection connectionInfo) {

    std::vector<networking::Connection> recipients;

    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
         recipients.emplace_back(connectionInfo);
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

    
    auto players = room.getAllPlayers();
    for (auto player : players) {
        recipients.emplace_back(player.getConnection());
    }
    return recipientsWrapper{recipients, Response{Status::SUCCESS, " left room"}};
    //this one later to show who left room
    // return recipientsWrapper{recipients, Response{Status::SUCCESS, player.getName() + " left room"}};
}

recipientsWrapper Controller::handleUserInput(json userInput) {
    // send userInput to room that player is in
    // json data to get room code
    // use roomCode to lookup room
    // 

    int roomCode = userInput["room_code"];
    //userInput["Player_info"];
    //userInput["data"];
}
