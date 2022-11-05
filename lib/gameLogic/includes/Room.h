#pragma once

#include <string_view>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "PlayerStorage.h"
#include "UniqueId.h"
#include "handler.h"

class RoomConfigInterface;

// room.h
// responsibilities:
// - keep track of players
// - store room info 
// - has rules on which players can join
class RoomInterface {
public:
    virtual int getNumPlayers() const;

    virtual bool addPlayer(PlayerInterface&);

    virtual void removePlayer(PlayerInterface&);

    virtual PlayerInterface& getPlayer(UniqueIdInterface&) const;
};


class Room : public RoomInterface {
public:
    Room(RoomConfigInterface* rc, PlayerStorageInterface* ps) :
        config{std::unique_ptr<RoomConfigInterface>(rc)}, 
        players{std::unique_ptr<PlayerStorageInterface>(ps)}
    { }

    int getNumPlayers() const override;

    bool addPlayer(PlayerInterface&) override;

    void removePlayer(PlayerInterface&) override;

    PlayerInterface& getPlayer(UniqueIdInterface&) const override;

private:
    std::unique_ptr<PlayerStorageInterface> players;
    std::unique_ptr<RoomConfigInterface> config;
};
 

class RoomConfigInterface {
public:
    virtual void setContext(RoomInterface*);

    virtual bool allow(PlayerInterface&) const;
};

class RoomConfig : public RoomConfigInterface {
public:

    int maxAllowedPlayers;
    int minAllowedPlayers;

    RoomConfig(Config);

    virtual void setContext(RoomInterface*) override;
    virtual bool allow(PlayerInterface&) const override;

private:
    std::unique_ptr<RoomInterface> context;

    void parseConfigRules(Config);
};
