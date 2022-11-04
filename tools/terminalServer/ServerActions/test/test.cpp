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