#include <string>
#include <memory>

#include <nlohmann/json.hpp>

#include "PlayerStorage.h"
#include "UniqueId.h"
#include "Room.h"
#include "handler.h"

using json = nlohmann::json;

int Room::getNumPlayers() const {
    return this->players->getNumPlayerRecords();
}

bool Room::addPlayer(PlayerInterface &p) {

    if (this->config->allow(p)) {

        this->players->addPlayerRecord(p);

        return true;
    }
    return false;
}

PlayerInterface& Room::getPlayer(UniqueIdInterface &pId) const {
    auto p = this->players->getPlayerRecord(pId);
    return p;
}

void Room::removePlayer(PlayerInterface &p) {
    auto pId = p.getPublicId();
    this->players->removePlayerRecord(pId);
}


void RoomConfig::setContext(RoomInterface* r) {
    this->context = std::unique_ptr<RoomInterface>(r);
}

bool RoomConfig::allow(PlayerInterface &p) const {
    return this->maxAllowedPlayers > this->context->getNumPlayers();
}

void RoomConfig::parseConfigRules(Config config) {
    this->maxAllowedPlayers = config.maxPlayer;
}
