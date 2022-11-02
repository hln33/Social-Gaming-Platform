#pragma once

#include <unordered_map>
#include <string>

#include "UniqueId.h"

// default hash

class IPlayer {
public:
    virtual void setPublicId(IUniqueId);

    virtual IUniqueId getPublicId() const;
};


class IPlayerStorage {
public:
    virtual void addPlayerRecord(IPlayer&);
    virtual void updatePlayerRecord(IPlayer&);
    virtual void removePlayerRecord(IUniqueId&);
    virtual const IPlayer& getPlayerRecord(IUniqueId&) const;
    virtual int getNumPlayerRecords() const;
};

class PlayerStorage : public IPlayerStorage {
public:
    virtual void addPlayerRecord(IPlayer&) override;
    virtual void updatePlayerRecord(IPlayer&);
    virtual void removePlayerRecord(IUniqueId&) override;
    virtual const IPlayer& getPlayerRecord(IUniqueId&) const override;
    virtual int getNumPlayerRecords() const override;

private:
    std::unordered_map<std::string, IPlayer> players;
};

