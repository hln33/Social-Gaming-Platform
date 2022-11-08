#pragma once

#include <string_view>
#include <string>
#include <memory>

#include "RoomConfig.h"
#include "Player.h"
#include "PlayerStorage.h"
#include "UniqueId.h"

class RoomConfig;

// room.h
// responsibilities:
// - keep track of players
// - store room info 
// - has rules on which players can join
// class IRoom {
// public:
//     virtual int getNumPlayers() const;

//     virtual bool addPlayer(Player&);

//     virtual void removePlayer(Player&);

//     virtual Player& getPlayer(IUniqueId&) const;
// };


class Room {
public:
    Room(std::unique_ptr<RoomConfig> roomConfig, std::unique_ptr<PlayerStorageInterface> playerStorage) : 
        config{std::move(roomConfig)}, 
        players{std::move(playerStorage)} 
    { }

    int getNumPlayers() const;

    bool addPlayer(Player&);

    void removePlayer(Player&);

    const Player& getPlayer(int) const;

    void updatePlayer(int, Player&);

private:
    std::unique_ptr<RoomConfig> config;
    std::unique_ptr<PlayerStorageInterface> players;
};

