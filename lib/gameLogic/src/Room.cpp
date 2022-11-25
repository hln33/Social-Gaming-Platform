#include <string>
#include <memory>
#include <algorithm>

#include <spdlog/spdlog.h>

#include "Room.h"


Response Room::addPlayer(Player& player) {
    if (this->config.satisfiesJoinPolicies(player, this->players)) {
        players.push_back(player);

        return Response {Status::Success, "add player ok"};
    }

    SPDLOG_ERROR("Failed to add player:{} into room", player.connectionID);
    throw Response {Status::Fail, "does not meet requiements"};
}

Response Room::removePlayer(Player& p) {
    auto removeThis = std::remove_if(players.begin(), players.end(), 
    [&p] (Player& player) { 
        return player.connectionID == p.connectionID;
    });
    if (removeThis == players.end()) {
        throw Response {Status::Fail, "Player not found"};
    }
 
    players.erase(removeThis);

    return Response {Status::Success, "Removed player" + p.connectionID};
}

Response Room::startGame(Player& requester) { 
    return Response {Status::Fail, "todo"};    
}

Response Room::endGame(Player& requester) {
    return Response {Status::Fail, "todo"};    
}

Response Room::sendGameData(Player& requester) {
    return Response {Status::Fail, "todo"};    
}

std::vector<Player> Room::getAllPlayers() {
    return players;
}

Player Room::findPlayer(uintptr_t connectionID) {
    auto playerItr = std::find_if(players.begin(), players.end(), [&connectionID](const auto& player){
        return player.connectionID == connectionID;
    });
    if (playerItr == players.end()) {
        SPDLOG_ERROR("could not find player in room");
        throw Response {Status::Fail, "Could not find player with ID: " + connectionID};
    }

    return *playerItr;
}

Player Room::getHost() {
    auto host = std::find_if(players.begin(), players.end(), 
        [] (auto player){
            return player.getplayerType() == playerTypeEnum::host;
        }
    );
    return *host;
}