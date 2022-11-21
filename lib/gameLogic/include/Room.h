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

class Room {
public:

    enum Status {
        Success,
        Fail,
    };

    struct StatusInfo {
        Status      statusCode;
        std::string message;
    };

    struct SendInfo {
        int         playerId;
        std::string message;
    };

    struct Response {
        StatusInfo            status;
        std::vector<SendInfo> sendInfo;
    };

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

    Player getHost();


private:
    RoomConfig config;
    std::vector<Player> players;
};


class ResponseBuilder {
public:
    ResponseBuilder(Room::Response& res) : 
        response{res}
    { }

    ResponseBuilder& setStatus(Room::Status status, std::string message) {
        this->response.status = Room::StatusInfo{status, message};
        return *this;
    }

    ResponseBuilder& addReceiver(Player receiver, std::string message) {
        this->response.sendInfo.push_back(Room::SendInfo{receiver.getId(), message});
        return *this;
    }

private:
    Room::Response& response;
};
