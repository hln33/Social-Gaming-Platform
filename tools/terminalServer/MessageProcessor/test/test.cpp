#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <ServerActions_pi.h>

TEST(create_json_message, create_basic_message) {
    json response = createJSONMessage("type", "message");

    EXPECT_TRUE(response["type"] == "type");
    EXPECT_TRUE(response["message"] == "message");
}

TEST(quit_game, quit_game__Test) {
    json response = quitGame(Connection{111});

    EXPECT_TRUE(response["type"] == "Player Left");
    EXPECT_TRUE(response["message"] == "111: Has left");
}

TEST(join_game, invalid_rules) {
    std::string rules = "";
    std::vector<Connection> recipients{};

    Connection host1{123};
    Connection host2{234};
    Connection creator{346};
    std::vector<Connection> hosts{host1, host2};
    std::map<std::string, std::vector<Connection>> rooms;
    std::map<uintptr_t, std::string> clientInfo;
    ServerAction::ServerDetails serverDetails{rooms, clientInfo, hosts};

    json response = createGame(rules, creator, recipients, serverDetails);

    EXPECT_TRUE(response["type"] == "Error");
    EXPECT_TRUE(response["message"] == "Error: Game rules are not in valid json format...");

    EXPECT_TRUE(hosts.size() != 0);
}