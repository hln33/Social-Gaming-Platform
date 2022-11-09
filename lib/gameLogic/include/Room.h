#pragma once

#include <string_view>
#include <string>
//#include <memory>
#include <nlohmann/json.hpp>
#include "Player.h"
#include "PlayerStorage.h"
#include "UniqueId.h"
#include <vector>
// #include "handler.h"

#include "RoomConfig.h"
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

enum RoomResponse {
    Success,
    Fail,
};

class Room {
public:
    Room(RoomConfigBuilderOptions roomConfig, Player host) { 
        config = buildRoomConfig(roomConfig, players);
        players.push_back(host);
    }

    // int getNumPlayers() const;

    RoomResponse addPlayer(Player);

    void removePlayer(Player);

    // Player getPlayer(int) const;


private:
    RoomConfig config;
    std::vector<Player> players;
};

