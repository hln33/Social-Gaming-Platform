#pragma once

#include <string_view>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

#include "Player.h"
#include "RoomConfig.h"
// room.h
// responsibilities:
// - keep track of players
// - store room info 
// - has rules on which players can join

enum Status {
    Success,
    Fail,
};

struct Response {
    Status code;
    std::string message;
};

class Room {
public:
    Room(RoomConfigBuilderOptions roomConfig, Player& host) { 
        buildRoomConfig(this->config, roomConfig, players);
        players.push_back(host);
    }

    Response addPlayer(Player&);

    Response removePlayer(Player&);

    Response startGame(Player&);

    Response endGame(Player&);

    Response sendGameData(Player&);

    std::vector<Player> getAllPlayers();

    Player findPlayer(uintptr_t connectionID);

    Player getHost();


private:
    RoomConfig config;
    std::vector<Player> players;
};
