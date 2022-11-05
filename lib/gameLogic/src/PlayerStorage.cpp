#include <unordered_map>

#include "UniqueId.h"
#include "PlayerStorage.h"

void PlayerStorage::addPlayerRecord(IPlayer &p) {
    this->players.insert(std::make_pair(p.getPublicId().getValue(), p));
}

void PlayerStorage::updatePlayerRecord(IPlayer &p) {
    this->players.insert_or_assign(p.getPublicId().getValue(), p);
}

void PlayerStorage::removePlayerRecord(IUniqueId& key) {
    this->players.erase(key.getValue());
}

const IPlayer& PlayerStorage::getPlayerRecord(IUniqueId& key) const {
    return this->players.at(key.getValue());
}

int PlayerStorage::getNumPlayerRecords() const {
    return this->players.size();
}
