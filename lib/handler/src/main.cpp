#include "../include/handler.h"
#include "../../gameLogic/include/gameLogic.h"
#include "../../gameLogic/include/room.h"
#include <string>

// use this main function to test handler code for now. In the future we will probably use unit tests instead.
int main() {
    
    // string passing example
    std::vector<Room> rooms;
    GameLogic gamelogic = {};

    std::string config1 = "{\"configuration\":{\"name\":\"ZenGame\",\"playercount\":{\"min\":0,\"max\":0},\"audience\":false,\"setup\":{}},\"constants\":{},\"variables\":{},\"per-player\":{},\"per-audience\":{},\"rules\":{}}";

    std::string config2 = R"(
    {
        "configuration": {
            "name": "Zen Game",
            "player count": {"min": "2", "max": "5"},
            "audience": false,
            "setup": { }
        },
        "constants": {},
        "variables": {},
        "per-player": {},
        "per-audience": {},
        "rules": {}
    }
    )";
    recieveMessage(config2, gamelogic);

    IRoomConfig config = gamelogic.getAllRooms().at(0).getConfig();
    std::cout << "number of rooms: " << gamelogic.getAllRooms().size() << "\n";

}