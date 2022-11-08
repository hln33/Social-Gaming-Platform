#pragma once

#include <unordered_map>
#include <string>

#include "UniqueId.h"

// default hash

class PlayerInterface {
public:
    virtual void setPublicId(UniqueIdInterface);

    virtual UniqueIdInterface getPublicId() const;
};


class PlayerStorageInterface {
public:
    virtual void addPlayerRecord(PlayerInterface&);
    virtual void updatePlayerRecord(PlayerInterface&);
    virtual void removePlayerRecord(UniqueIdInterface&);
    virtual const PlayerInterface& getPlayerRecord(UniqueIdInterface&) const;
    virtual int getNumPlayerRecords() const;
};

class PlayerStorage : public PlayerStorageInterface {
public:
    virtual void addPlayerRecord(PlayerInterface&) override;
    virtual void updatePlayerRecord(PlayerInterface&);
    virtual void removePlayerRecord(UniqueIdInterface&) override;
    virtual const PlayerInterface& getPlayerRecord(UniqueIdInterface&) const override;
    virtual int getNumPlayerRecords() const override;

private:
    std::unordered_map<std::string, PlayerInterface> players;
};

