#include <string>

#include "player_storage.h"

#include "unique_id.h"
#include "unique_id_builder.h"

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
    Room(IRoomConfig, IPlayerStorage);

    int getNumPlayers() const override;

    bool addPlayer(IPlayer&) override;

    void removePlayer(IPlayer&) override;

    IPlayer& getPlayer(IUniqueId&) const override;

private:
    IPlayerStorage players;
    IRoomConfig config;
};

class IRoomConfig {
public:
    virtual void setContext(Room&);

    virtual bool allow(IPlayer&) const;
};

class RoomConfig : public IRoomConfig {
public:

    int maxAllowedPlayers;

    RoomConfig(JObject);

    virtual void setContext(Room&) override;
    virtual bool allow(IPlayer&) const override;

private:
    Room &context;

    void parseConfigRules(JObject);
};

class JObject {
public:
    std::string get(std::string_view);
};
