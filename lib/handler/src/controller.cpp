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

    while(it == GameRoomLookUp.end()){
        newRandomCode = randomCode();
    }

    return newRandomCode;
}

Room Controller::createRoom(json jsonFile, Player host) {
    RoomConfigBuilderOptions configBuilder = extractConfig(jsonFile);

    std::vector<Player> players;
    RoomConfig RoomConfig = buildRoomConfig(configBuilder, players);

    return Room{configBuilder, host};
}

////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC
////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: might not need the client to store the player id because we can use the connection info
// instead to uniquely determine which player


Response Controller::createRoom(json jsonFile, networking::Connection connectionObj) {    
    //make host and add them to the player hashtable
    Player newHost = Player{playerTypeEnum::host};
    playerLookUp.insert({connectionObj, newHost.getId()});
    
    // 1. generate random code
    std::string newRandomCode = generateRoomCode();

    // 2. add the room into GameRoomLookup
    Room room = createRoom(jsonFile, newHost);
    GameRoomLookUp.insert(std::pair<std::string, Room>(newRandomCode, std::move(room)));
    
     // // 3. return the response
    return Response{Status::SUCCESS, newRandomCode};
}


// Response Controller::createRoomConfig(json jsonFile){
//     if (jsonFile.contains("configuration")){

//         // we cannot pass json object to create config because that contradicts:
//         // business do not need to handle json parsing

//         // the argument should be struct
//         // Config c;
//         // RoomConfig config = extractConfig(jsonFile);


//         // return config;
//     }
//     return Response{Status::SUCCESS, "RANDOM CODE"};

// }

Response Controller::joinRoom(std::string roomCode, networking::Connection connectionInfo) {
    // 1. lookup the room in GameRoomLookup
    // 1.1 if could not find room send error response
    // 1.2 otherwise ask the room if player can join
    // 1.2.1 if no then send the response
    // 1.2.2 if yes then send the response

    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
        return Response{Status::FAIL, "Could not find room!"};
    }
    
    Room& room = (*roomItr).second;
    Player newPlayer {playerTypeEnum::player};
    if (!room.addPlayer(newPlayer)) {
        return Response{Status::FAIL, "Not allowed to join the room!"};
    }

    return Response{Status::SUCCESS, "Join room ok"};
}

Response Controller::leaveRoom(std::string roomCode, networking::Connection connectionInfo) {
    auto roomItr = GameRoomLookUp.find(roomCode);
    if (roomItr == GameRoomLookUp.end()) {
        return Response{Status::FAIL, "Could not find room!"};
    }

    Room& room = (*roomItr).second; 
    //room.removePlayer(playerInfo);

    return Response{Status::SUCCESS, "Left room ok"};
}

Response Controller::handleUserInput(json userInput) {
    // send userInput to room that player is in
    // json data to get room code
    // use roomCode to lookup room
    // 

    int roomCode = userInput["room_code"];
    //userInput["Player_info"];
    //userInput["data"];
}
