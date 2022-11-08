// Desription 

#include "controller.h"
#include "handlerHelper.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC
////////////////////////////////////////////////////////////////////////////////////////////////

Response Controller::createRoom(json jsonFile, networking::Connection player){

    // void config = createRoomConfig(jsonFile);
    // Player::Player newHost()
    
    // steps:
    // 1. generate random code
    // 2. add the room into GameRoomLookup
    // 3. create the response object 
    // // 4. return the response
    // std::string newRandomCode = randomCode();
    // auto it = GameRoomLookUp.find(newRandomCode);

    // while(it == GameRoomLookUp.end()){
    //     newRandomCode = randomCode();
    // }

    // // Room::Room newRoom(config, newHost, newRandomCode);


    
    
    return Response{Status::SUCCESS, "RANDOM CODE"};
}


Response Controller::createRoomConfig(json jsonFile){
    if (jsonFile.contains("configuration")){

        // we cannot pass json object to create config because that contradicts:
        // business do not need to handle json parsing

        // the argument should be struct
        // Config c;
        // RoomConfig config = extractConfig(jsonFile);


        // return config;
    }
    return Response{Status::SUCCESS, "RANDOM CODE"};

}

Response Controller::joinRoom(std::string roomCode, Player& playerInfo) {
    // 1. lookup the room in GameRoomLookup
    // 1.1 if could not find room send error response
    // 1.2 otherwise ask the room if player can join
    // 1.2.1 if no then send the response
    // 1.2.2 if yes then send the response


    auto roomItr = this->GameRoomLookUp.find(roomCode);
    if (roomItr != this->GameRoomLookUp.end()) {
        Room& room = (*roomItr).second;
        // ask the room if player can join
        if (!room.addPlayer(playerInfo)) {
            // room doesnt allow the player
            return Response{Status::FAIL, "Not allowed to join the room!"};
        }

        // allow the player
        return Response{Status::SUCCESS, "Join room ok"};
    }

    // could not find room 
    // send error response
    return Response{Status::FAIL, "Could not find room!"};
}

Response Controller::leaveRoom(std::string roomCode, Player& playerInfo) {
    auto it = this->GameRoomLookUp.find(roomCode);
    if (it == this->GameRoomLookUp.end()) {
        return Response{Status::FAIL, "Could not find room!"};
    }

    Room& room = (*it).second;
    room.removePlayer(playerInfo);
    if (room.getNumPlayers() == 0) {
        this->deleteRoom(roomCode);
    }

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
