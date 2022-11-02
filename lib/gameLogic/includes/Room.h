#pragma once

#include <string_view>
#include <string>
#include <memory>

#include "PlayerStorage.h"
#include "UniqueId.h"

class IRoomConfig;

// room.h
// responsibilities:
// - keep track of players
// - store room info 
// - has rules on which players can join
class IRoom {
public:
    virtual int getNumPlayers() const;

    virtual bool addPlayer(IPlayer&);

    virtual void removePlayer(IPlayer&);

    virtual IPlayer& getPlayer(IUniqueId&) const;
};


class Room : public IRoom {
public:
    Room(std::unique_ptr<IRoomConfig>, std::unique_ptr<IPlayerStorage>);

    int getNumPlayers() const override;

    bool addPlayer(IPlayer&) override;

    void removePlayer(IPlayer&) override;

    IPlayer& getPlayer(IUniqueId&) const override;

private:
    std::unique_ptr<IPlayerStorage> players;
    std::unique_ptr<IRoomConfig> config;
};

// fake json object
class JObject {
public:
    std::string get(std::string);
};


class IRoomConfig {
public:
    virtual void setContext(std::unique_ptr<IRoom>);

    virtual bool allow(IPlayer&) const;
};

class RoomConfig : public IRoomConfig {
public:

    int maxAllowedPlayers;

    RoomConfig(JObject);

    virtual void setContext(std::unique_ptr<IRoom>) override;
    virtual bool allow(IPlayer&) const override;

private:
    std::unique_ptr<IRoom> context;

    void parseConfigRules(JObject);
};

