#include <gtest/gtest.h>
#include <fstream>
#include <handler.h>

TEST(exampleTests, DemonstrateGTestMacros) {
    EXPECT_TRUE(true);
    EXPECT_TRUE(2 == 2);

    std::string test;
    recieveMessage(test);
}

TEST(exampleTests, parseRockPaperScissorFromJSONFile) {

    // example of parsing JSON from a file
    // Source: https://github.com/nlohmann/json#read-json-from-a-file

    // modify this path to yours path to read the file
    std::ifstream file("/home/nhanvyn/sg/social-gaming/lib/handler/test/rock-paper-scissor.json");  
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string test = buffer.str();
    bool result = recieveMessage(test);
    EXPECT_TRUE(result);
}


TEST(exampleTests, parseRockPaperScissorFromString) {
    
    // example of parsing JSON directly from string
    // copy from project description
    std::string rock_paper_scissor = R"(
    {
        "configuration": {
            "name": "Rock, Paper, Scissors",
            "player count": {
                "min": 2,
                "max": 4
            },
            "audience": false,
            "setup": {
                "Rounds": 10
            }
        },

        "constants": {
            "weapons": [{
                    "name": "Rock",
                    "beats": "Scissors"
                },
                {
                    "name": "Paper",
                    "beats": "Rock"
                },
                {
                    "name": "Scissors",
                    "beats": "Paper"
                }
            ]
        },

        "variables": {
            "winners": []
        },

        "per-player": {
            "wins": 0
        },

        "per-audience": {},

        "rules": [{
                "rule": "foreach",
                "list": "configuration.Rounds.upfrom(1) ",
                "element": "round",
                "rules": [

                    {
                        "rule": "global-message",
                        "value": "Round {round}. Choose your weapon!"
                    },

                    {
                        "rule": "parallelfor",
                        "list": "players",
                        "element": "player",
                        "rules": [

                            {
                                "rule": "input-choice",
                                "to": "player",
                                "prompt": "{player.name}, choose your weapon!",
                                "choices": "weapons.name",
                                "result": "player.weapon",
                                "timeout": 10
                            }

                        ]
                    },

                    {
                        "rule": "discard",
                        "from": "winners",
                        "count": "winners.size"
                    },

                    {
                        "rule": "foreach",
                        "list": "weapons",
                        "element": "weapon",
                        "rules": [

                            {
                                "rule": "when",
                                "cases": [{
                                    "condition": "!players.elements.weapon.contains(weapon.name) ",
                                    "rules": [

                                        {
                                            "rule": "extend",
                                            "target": "winners",
                                            "list": "players.elements.collect(player, player.weapon == weapon.beats) "
                                        }

                                    ]
                                }]
                            }

                        ]
                    },

                    {
                        "rule": "when",
                        "cases": [{
                                "condition": "winners.size == players.size",
                                "rules": [{
                                    "rule": "global-message",
                                    "value": "Tie game!"
                                }]
                            },
                            {
                                "condition": "winners.size == 0",
                                "rules": [{
                                    "rule": "global-message",
                                    "value": "Tie game!"
                                }]
                            },
                            {
                                "condition": true,
                                "rules": [{
                                        "rule": "global-message",
                                        "value": "Winners: {winners.elements.name}"
                                    },
                                    {
                                        "rule": "foreach",
                                        "list": "winners",
                                        "element": "winner",
                                        "rules": [{
                                            "rule": "add",
                                            "to": "winner.wins",
                                            "value": 1
                                        }]
                                    }
                                ]
                            }
                        ]
                    }

                ]
            },

            {
                "rule": "scores",
                "score": "wins",
                "ascending": false
            }
        ]
    }
    )";
    bool result = recieveMessage(rock_paper_scissor);
    EXPECT_TRUE(result);
}