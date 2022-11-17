#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "controller.h"

using namespace testing;

class ControllerTests : public Test {
    protected:
        void SetUp() override { }

    Controller controller;
    json config = json::parse(R"(
        {
            "configuration": {
                "name": "Rock, Paper, Scissors",
                "player count": {"min": 2, "max": 4},
                "audience": false,
                "setup": {
                    "Rounds": 10
                }
            }
        }
    )");
};

TEST_F(ControllerTests, createRoom) {
    networking::Connection player;
    player.id = 111;

    recipientsWrapper response = controller.createRoom(config, player);
    auto recipients = response.recipientList;

    EXPECT_EQ(response.data.status, Status::SUCCESS);
    EXPECT_FALSE(response.data.message == ""); // game code should created
    EXPECT_FALSE(recipients.find(player) == recipients.end());
    EXPECT_EQ(recipients.size(), 1);
}