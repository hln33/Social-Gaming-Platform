#include <string>
#include <memory>

#include <nlohmann/json.hpp>

#include "PlayerStorage.h"
#include "UniqueId.h"
#include "Room.h"
// #include "handler.h"

using json = nlohmann::json;

int Room::getNumPlayers() const {
    return this->players->getNumPlayerRecords();
}

bool Room::addPlayer(Player& p) {

    if (this->config->satisfiesJoinPolicies(p)) {

        this->players->addPlayerRecord(std::make_unique<Player>(p));

        return true;
    }
    return false;
}

const Player& Room::getPlayer(int pId) const {
    const Player* p = this->players->getPlayerRecord(pId);
    return *p;
}

void Room::updatePlayer(int pId, Player& player) {
    this->players->updatePlayerRecord(pId, std::make_unique<Player>(player));
}

void Room::removePlayer(Player& p) {
    auto pId = p.getId();
    this->players->removePlayerRecord(pId);
}
