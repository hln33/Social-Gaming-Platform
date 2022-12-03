#include <string>
#include <memory>
#include <algorithm>

#include <spdlog/spdlog.h>

#include "Room.h"


void Room::addPlayer(Player& player) {
    if (this->config.satisfiesJoinPolicies(player, this->players)) {
        players.push_back(player);
        return;
    }

    SPDLOG_ERROR("Failed to add player:{} into room", player.connectionID);
    throw Response {REQUIREMENTS_NOT_MET};
}

void Room::removePlayer(Player& p) {
    auto removeThis = std::remove_if(players.begin(), players.end(), 
    [&p] (Player& player) { 
        return player.connectionID == p.connectionID;
    });
    if (removeThis == players.end()) {
        throw Response {PLAYER_NOT_FOUND_IN_ROOM};
    }
  
    players.erase(removeThis);
}

Response Room::startGame(Player& requester) { 
    return Response {"todo"};    
}

Response Room::endGame(Player& requester) {
    return Response {"todo"};    
}

Response Room::sendGameData(Player& requester) {
    return Response {"todo"};    
}

Response Room::inputChoice(json data) {
    return Response {"todo"};
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
        throw Response {PLAYER_NOT_FOUND_IN_ROOM};
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