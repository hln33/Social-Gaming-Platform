#include <unordered_map>

#include "UniqueId.h"
#include "PlayerStorage.h"

void PlayerStorage::addPlayerRecord(PlayerInterface &p) {
    this->players.insert(std::make_pair(p.getPublicId().getValue(), p));
}

void PlayerStorage::updatePlayerRecord(PlayerInterface &p) {
    this->players[p.getPublicId().getValue()] = p;
}

void PlayerStorage::removePlayerRecord(UniqueIdInterface& key) {
    this->players.erase(key.getValue());
}

const PlayerInterface& PlayerStorage::getPlayerRecord(UniqueIdInterface& key) const {
    return this->players.at(key.getValue());
}

int PlayerStorage::getNumPlayerRecords() const {
    return this->players.size();
}
