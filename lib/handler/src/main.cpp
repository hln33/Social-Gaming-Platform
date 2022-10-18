#include "../include/handler.h"
#include <string>
#include <iostream>
// use this main function to test handler code for now. In the future we will probably use unit tests instead.
int main() {
    // std::string test;
    // recieveMessage(test);
   
    std::string valid_config = R"(
    {
        "configuration": {
            "name": "Zen Game",
            "player count": {"min": 2, "max": 5},
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
    recieveMessage(valid_config);

}