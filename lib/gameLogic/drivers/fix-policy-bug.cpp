
#include <iostream>
#include <vector>
#include <string>

#include "RoomConfig.h"
#include "RoomPolicy.h"


#include "Player.h"


int main() {

    std::vector<Player> players = {
        Player{playerTypeEnum::host, "hostplayer"},
        Player{playerTypeEnum::player, "player1"},
        Player{playerTypeEnum::player, "player2"}
    };

    Player nonAudience = Player{playerTypeEnum::player, "ma23"};
    Player audience = Player{playerTypeEnum::audience, "ma3566"};

    RoomConfigBuilderOptions conf;
    conf.name = "test room";
    conf.maxPlayers = 4;
    conf.minPlayers = -1;
    conf.allowAudience = false;

    RoomConfig roomConf = RoomConfig{};

    buildRoomConfig(roomConf, conf, players);

    std::cout << "size of players: " << players.size() << std::endl;

    // add audience player -> expected to fail
    std::cout << "expected: rejected, " << "actual: " << (roomConf.satisfiesJoinPolicies(audience, players) ? "allowed\n" : "rejected\n");
    std::cout << "reason: " << roomConf.result << std::endl;
    // std::cout << "size of players: " << players.size() << std::endl;

    // add non audience should pass
    std::cout << "expected: allowed, " << "actual: " << (roomConf.satisfiesJoinPolicies(nonAudience, players) ? "allowed\n" : "rejected\n");
    // std::cout << "reason: " << roomConf.result << std::endl;
    // std::cout << "size of players: " << players.size() << std::endl;
    players.push_back(nonAudience);

    // add player violating max players -> expect fail
    Player anotherPlayer = Player{playerTypeEnum::player, "jack2324"};
    std::cout << "expected: rejected, " << "actual: " << (roomConf.satisfiesJoinPolicies(anotherPlayer, players) ? "allowed\n" : "rejected\n");
    std::cout << "reason: " << roomConf.result << std::endl;
    // std::cout << "size of players: " << players.size() << std::endl;

    return 0;
}

