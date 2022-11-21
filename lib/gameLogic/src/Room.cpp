#include <string>
#include <memory>
#include <algorithm>

#include <spdlog/spdlog.h>

#include "Room.h"


room::Response Room::addPlayer(Player& player) {
    if (this->config.satisfiesJoinPolicies(player, this->players)) {
        players.push_back(player);

        room::Response res{room::Status::Success, "add player ok"};
        return res;
    }
    room::Response res{room::Status::Fail, "does not meet requiements"};
    return res;
}

room::Response Room::removePlayer(Player& p) {
    auto removeThis = std::remove_if(players.begin(), players.end(), 
    [&p] (Player& player) { 
        return player.connectionID == p.connectionID;
    });
 
    players.erase(removeThis);

    room::Response res{room::Status::Success, "Removed player" + p.connectionID};
    return res;
}

room::Response Room::startGame(Player& requester) { 
    room::Response res{room::Status::Fail, "todo"};    
    return res;
}

room::Response Room::endGame(Player& requester) {
    room::Response res{room::Status::Fail, "todo"};    
    return res;
}

room::Response Room::sendGameData(Player& requester) {
    room::Response res{room::Status::Fail, "todo"};    
    return res;
}

std::vector<Player> Room::getAllPlayers() {
    return players;
}

Player Room::getHost() {
    auto host = std::find_if(players.begin(), players.end(), 
        [] (auto player){
            return player.getplayerType() == playerTypeEnum::host;
        }
    );
    return *host;
}