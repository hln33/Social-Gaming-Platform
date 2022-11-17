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
            player5.id = 555;

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
    networking::Connection player5;

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

    auto it = recipients.find(player2);
    auto id = (*it).id;
    EXPECT_FALSE(it == recipients.end());
    EXPECT_EQ(id, 222);

    it = recipients.find(player1);
    id = (*it).id;
    EXPECT_FALSE(it == recipients.end());
    EXPECT_EQ(id, 111);
}

TEST_F(ControllerTests, multiplePlayersJoinRoom) {
    auto response = controller.joinRoom(inviteCode, player2);
    auto recipients = response.recipientList;
    EXPECT_EQ(response.data.status, Status::SUCCESS);
    EXPECT_EQ(recipients.size(), 2);

    response = controller.joinRoom(inviteCode, player3);
    recipients = response.recipientList;
    EXPECT_EQ(response.data.status, Status::SUCCESS);
    EXPECT_EQ(recipients.size(), 3);

    response = controller.joinRoom(inviteCode, player4);
    recipients = response.recipientList;
    EXPECT_EQ(response.data.status, Status::SUCCESS);
    EXPECT_EQ(recipients.size(), 4);

    auto it = recipients.find(player4);
    auto id = (*it).id;
    EXPECT_FALSE(it == recipients.end());
    EXPECT_EQ(id, 444);

    it = recipients.find(player3);
    id = (*it).id;
    EXPECT_FALSE(it == recipients.end());
    EXPECT_EQ(id, 333);

    it = recipients.find(player2);
    id = (*it).id;
    EXPECT_FALSE(it == recipients.end());
    EXPECT_EQ(id, 222);
}

TEST_F(ControllerTests, maxPlayersInRoom) {
    controller.joinRoom(inviteCode, player2);
    controller.joinRoom(inviteCode, player3);
    controller.joinRoom(inviteCode, player4);

    auto response = controller.joinRoom(inviteCode, player5);
    auto recipients = response.recipientList;
    EXPECT_EQ(response.data.status, Status::FAIL);
    EXPECT_EQ(recipients.size(), 1);

    auto it = recipients.find(player5);
    auto id = (*it).id;
    EXPECT_FALSE(it == recipients.end());
    EXPECT_EQ(id, 555);
}

TEST_F(ControllerTests, onePlayerLeavesRoom) {

}

TEST_F(ControllerTests, multiplePlayersLeaveRoom) {

}

TEST_F(ControllerTests, allPlayersLeaveRoom) {

}