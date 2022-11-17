#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <spdlog/spdlog.h>

#include "controller.h"

using namespace testing;

class ControllerTests : public Test {
    protected:
        void SetUp() override {
            player1.id = 111;
            player2.id = 222;
            player3.id = 333;
            player4.id = 444;

            auto response = controller.createRoom(config, player1);
            inviteCode = response.data.message;
        }

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

    networking::Connection player1;
    networking::Connection player2;
    networking::Connection player3;
    networking::Connection player4;

    std::string inviteCode;
};

TEST_F(ControllerTests, createRoom) {
    auto response = controller.createRoom(config, player1);
    auto recipients = response.recipientList;

    EXPECT_EQ(response.data.status, Status::SUCCESS);
    EXPECT_FALSE(response.data.message == ""); // game code should created
    EXPECT_FALSE(recipients.find(player1) == recipients.end());
    EXPECT_EQ(recipients.size(), 1);
}

TEST_F(ControllerTests, onePlayerJoinsRoom) {
    auto response = controller.joinRoom(inviteCode, player2);
    auto recipients = response.recipientList;

    EXPECT_EQ(response.data.status, Status::SUCCESS);
    EXPECT_EQ(recipients.size(), 2);

    for_each(recipients.begin(), recipients.end(), [](const networking::Connection& recipient){
        SPDLOG_INFO(recipient.id);
    });

    EXPECT_FALSE(recipients.find(player2) == recipients.end());
}

TEST_F(ControllerTests, multiplePlayersJoinRoom) {
    auto response = controller.joinRoom(inviteCode, player2);
    auto recipients = response.recipientList;

    EXPECT_EQ(response.data.status, Status::SUCCESS);
    EXPECT_EQ(recipients.size(), 2);
    EXPECT_FALSE(recipients.find(player2) == recipients.end());

    response = controller.joinRoom(inviteCode, player3);
    recipients = response.recipientList;

    EXPECT_EQ(response.data.status, Status::SUCCESS);
    EXPECT_EQ(recipients.size(), 3);
    EXPECT_FALSE(recipients.find(player3) == recipients.end());

    response = controller.joinRoom(inviteCode, player4);
    recipients = response.recipientList;

    EXPECT_EQ(response.data.status, Status::SUCCESS);
    EXPECT_EQ(recipients.size(), 4);
    EXPECT_FALSE(recipients.find(player4) == recipients.end());

    EXPECT_FALSE(recipients.find(player3) == recipients.end());
    EXPECT_FALSE(recipients.find(player2) == recipients.end());
}