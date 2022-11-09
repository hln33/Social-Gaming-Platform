#include <string>
#include <memory>

#include <nlohmann/json.hpp>
#include <algorithm>
#include "PlayerStorage.h"
#include "UniqueId.h"
#include "Room.h"
// #include "handler.h"

using json = nlohmann::json;

// int Room::getNumPlayers() const {
//     return this->players->size();
// }



RoomResponse Room::addPlayer(Player player) {

    if (this->config.satisfiesJoinPolicies(player)) {
        players.push_back(player);
        return RoomResponse::Success;
    }
    return RoomResponse::Fail;
}

// Player Room::getPlayer(int pId) const {
//     auto p = std::find_if(players.begin(), players.end(), 
//     [&pId] (Player& p) { 
//         return p.getId() == pId;
//     });

    
    
//     return *p;
// }


void Room::removePlayer(Player p) {

    auto removeThis = std::remove_if(players.begin(), players.end(), 
    [&p] (Player& player) { 
        return player.getId() == p.getId();
    });

    players.erase(removeThis);
}
