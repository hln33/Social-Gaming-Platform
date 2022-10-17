#pragma once
#include <unordered_map>

#include "unique_id.h"

// default hash
class IPlayer {
public:
    virtual void setPublicId(IUniqueId);

    virtual IUniqueId getPublicId() const;
};

namespace std {
    template<>
    struct hash<IPlayer> {
        std::size_t operator()(IPlayer const& p) const noexcept {
            return std::hash<std::string>{}(p.getPublicId().getValue());
        }
    };
}

class IPlayerStorage {
public:
    // virtual void addPlayerRecord(IPlayer&);
    // virtual void updatePlayerRecord(IPlayer&);
    // virtual void removePlayerRecord(IUniqueId&);
    // virtual const IPlayer& getPlayerRecord(IUniqueId&) const;
    // virtual int getNumPlayerRecords() const;
};

class PlayerStorage : public IPlayerStorage {
public:
    PlayerStorage(){}
    // virtual void addPlayerRecord(IPlayer&) override;
    // virtual void updatePlayerRecord(IPlayer&);
    // virtual void removePlayerRecord(IUniqueId&) override;
    // virtual const IPlayer& getPlayerRecord(IUniqueId&) const override;
    // virtual int getNumPlayerRecords() const override;
    
private:
    // std::unordered_map<IUniqueId, IPlayer> players;
};

