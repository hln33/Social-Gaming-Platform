#include "../include/handler.h"
#include <string>
#include <iostream>
// use this main function to test handler code for now. In the future we will probably use unit tests instead.
int main() {
    // std::string test;
    // recieveMessage(test);
   
    std::string simple_config = R"(
    {
        "configuration": {
            "name": "Zen Game",
            "player count": {"min": 2, "max": 5},
            "audience": false,
            "setup": { }
        },
        "constants": {
             "weapons":[
                {
                    "name":"Rock",
                    "beats":"Scissors"
                },
                {
                    "name":"Paper",
                    "beats":"Rock"
                },
                {
                    "name":"Scissors",
                    "beats":"Paper"
                }
            ]
        },
        "variables": {
            "winners":[]
        },
        "per-player": {},
        "per-audience": {},
        "rules": [
            {
               "rule":"global-message",
               "value":"Round {round}. Choose your weapon!"
            },
            {
               "rule":"parallelfor",
               "list":"players",
               "element":"player",
               "rules":[
                  {
                     "rule":"input-choice",
                     "to":"player",
                     "prompt":"{player.name}, choose your weapon!",
                     "choices":"weapons.name",
                     "result":"player.weapon",
                     "timeout":10
                  }
               ]
            },
            {
               "rule":"discard",
               "from":"winners",
               "count":"winners.size"
            }
        ]
    }
    )";
    recieveMessage(simple_config);

}