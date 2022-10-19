#include <unordered_map>

#include "includes/unique_id.h"
#include "includes/player_storage.h"

void PlayerStorage::addPlayerRecord(IPlayer &p) {
    this->players.insert(std::make_pair(p.getPublicId(), p));
}

void PlayerStorage::updatePlayerRecord(IPlayer &p) {
    this->players.insert_or_assign(p.getPublicId(), p);
}

void PlayerStorage::removePlayerRecord(IUniqueId& key) {
    this->players.erase(key);
}

const IPlayer& PlayerStorage::getPlayerRecord(IUniqueId& key) const {
    return this->players.at(key);
}

int PlayerStorage::getNumPlayerRecords() const {
    return this->players.size();
}
