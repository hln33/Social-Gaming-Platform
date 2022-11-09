#include <unordered_map>
#include <memory>

#include "UniqueId.h"
#include "PlayerStorage.h"

void PlayerStorage::addPlayerRecord(std::unique_ptr<Player> p) {
    this->players.insert(std::make_pair(p->getId(), std::move(p)));
}

void PlayerStorage::updatePlayerRecord(int id, std::unique_ptr<Player> p) {
    this->players.insert_or_assign(id, std::move(p));
}

void PlayerStorage::removePlayerRecord(int key) {
    this->players.erase(key);
}

const Player* PlayerStorage::getPlayerRecord(int key) const {
    return this->players.at(key).get();
}

size_t PlayerStorage::getNumPlayerRecords() const {
    return this->players.size();
}

std::unique_ptr<PlayerStorageInterface> buildPlayerStorage() {
    return std::make_unique<PlayerStorage>();
}

