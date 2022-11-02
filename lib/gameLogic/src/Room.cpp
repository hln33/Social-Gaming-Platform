#include <string>
#include <memory>

#include "../includes/PlayerStorage.h"
#include "../includes/UniqueId.h"
#include "../includes/Room.h"

Room::Room(std::unique_ptr<IRoomConfig> rc, std::unique_ptr<IPlayerStorage> ps) : 
    config{std::move(rc)}, players{std::move(ps)} {

}

int Room::getNumPlayers() const {
    return this->players->getNumPlayerRecords();
}

bool Room::addPlayer(IPlayer &p) {

    if (this->config->allow(p)) {

        this->players->addPlayerRecord(p);

        return true;
    }
    return false;
}

IPlayer& Room::getPlayer(IUniqueId &pId) const {
    auto p = this->players->getPlayerRecord(pId);
    return p;
}

void Room::removePlayer(IPlayer &p) {
    auto pId = p.getPublicId();
    this->players->removePlayerRecord(pId);
}


void RoomConfig::setContext(std::unique_ptr<IRoom> r) {
    this->context = std::move(r);
}

bool RoomConfig::allow(IPlayer &p) const {
    return this->maxAllowedPlayers > this->context->getNumPlayers();
}

void RoomConfig::parseConfigRules(JObject jsonRules) {
    auto maxPlayers = jsonRules.get("max-players");
    this->maxAllowedPlayers = std::stoi(maxPlayers);
}
