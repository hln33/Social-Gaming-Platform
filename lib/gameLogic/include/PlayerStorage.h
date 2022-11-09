#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Player.h"
#include "UniqueId.h"

// default hash

// class IPlayer {
// public:
//     virtual void setPublicId(IUniqueId);

//     virtual IUniqueId getPublicId() const;
// };


class PlayerStorageInterface {
public:
    virtual void addPlayerRecord(std::unique_ptr<Player>);
    virtual void updatePlayerRecord(int, std::unique_ptr<Player>);
    virtual void removePlayerRecord(int);
    virtual const Player* getPlayerRecord(int) const;
    virtual size_t getNumPlayerRecords() const;
};

class PlayerStorage : public PlayerStorageInterface {
public:
    void addPlayerRecord(std::unique_ptr<Player>) override;
    void updatePlayerRecord(int, std::unique_ptr<Player>) override;
    void removePlayerRecord(int) override;
    const Player* getPlayerRecord(int) const override;
    size_t getNumPlayerRecords() const override;
private:
    std::unordered_map<int, std::unique_ptr<Player>> players;
};


std::unique_ptr<PlayerStorageInterface> buildPlayerStorage();

