#include <string>
#include <memory>
#include <algorithm>

#include "Room.h"


Room::Response Room::addPlayer(Player player) {
    Response res;
    ResponseBuilder builder{res};

    if (this->config.satisfiesJoinPolicies(player, this->players)) {
        players.push_back(player);

        builder.setStatus(Status::Success, "add player ok");
        for (auto p : players) {
            builder.addReceiver(p, "player " + player.getName() + " joined the room");
        }
        return res;
    }
    builder.setStatus(Status::Fail, "does not meet requirements");

    return res;
}

Room::Response Room::removePlayer(Player p) {
    Response res;
    ResponseBuilder builder{res};

    auto removeThis = std::remove_if(players.begin(), players.end(), 
    [&p] (Player& player) { 
        return player.getId() == p.getId();
    });

    players.erase(removeThis);

    builder.setStatus(Status::Success, "removed player " + p.getName());
    return res;
}

Room::Response Room::startGame(Player requester) { 
    Room::Response res = Room::Response{};

    // TODO

    auto builder = ResponseBuilder{res};
    builder.setStatus(Room::Status::Fail, "todo");

    return res;
}

Room::Response Room::sendGameData(Player requester) {
    Room::Response res = Room::Response{};

    // TODO

    auto builder = ResponseBuilder{res};
    builder.setStatus(Room::Status::Fail, "todo");

    return res;
}

std::vector<Player> Room::getAllPlayers() {
    return players;
}

Player Room::getHost() {
    auto host = std::find_if(players.begin(), players.end(), 
        [] (auto player){
            return player.getplayerType() == playerTypeEnum::host;
        }
    );
    return *host;
}