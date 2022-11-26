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

    EXPECT_EQ(response.responseCode, ResponseCode::CREATE_GAME_SUCCESS);
    EXPECT_FALSE(response.data.message == ""); // game code should created
    EXPECT_FALSE(recipients.find(player1) == recipients.end());
    EXPECT_EQ(recipients.size(), 1);
}

TEST_F(ControllerTests, onePlayerJoinsRoom) {
    auto response = controller.joinRoom(inviteCode, player2);
    auto recipients = response.recipientList;

    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);
    EXPECT_EQ(recipients.size(), 2);

    auto it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player1);
    EXPECT_FALSE(it == recipients.end());
}

TEST_F(ControllerTests, multiplePlayersJoinRoom) {
    auto response = controller.joinRoom(inviteCode, player2);
    auto recipients = response.recipientList;
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);
    EXPECT_EQ(recipients.size(), 2);

    response = controller.joinRoom(inviteCode, player3);
    recipients = response.recipientList;
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);
    EXPECT_EQ(recipients.size(), 3);

    response = controller.joinRoom(inviteCode, player4);
    recipients = response.recipientList;
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);
    EXPECT_EQ(recipients.size(), 4);

    auto it = recipients.find(player4);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player3);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());
}

TEST_F(ControllerTests, maxPlayersInRoom) {
    controller.joinRoom(inviteCode, player2);
    controller.joinRoom(inviteCode, player3);
    controller.joinRoom(inviteCode, player4);

    auto response = controller.joinRoom(inviteCode, player5);
    auto recipients = response.recipientList;
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_FAIL);
    EXPECT_EQ(recipients.size(), 1);

    auto it = recipients.find(player5);
    EXPECT_FALSE(it == recipients.end());
}

TEST_F(ControllerTests, onePlayerLeavesRoom) {
    auto response = controller.joinRoom(inviteCode, player2);
    auto recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 2);
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);

    auto it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player1);
    EXPECT_FALSE(it == recipients.end());

    response = controller.leaveRoom(player2);
    recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 2);
    EXPECT_EQ(response.responseCode, ResponseCode::LEFT_ROOM_SUCCESS);
}

TEST_F(ControllerTests, multiplePlayersLeaveRoom) {
    auto response = controller.joinRoom(inviteCode, player2);
    auto recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 2);
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);

    auto it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());


    response = controller.joinRoom(inviteCode, player3);
    recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 3);
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);

    it = recipients.find(player3);
    EXPECT_FALSE(it == recipients.end());


    response = controller.leaveRoom(player2);
    recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 3);
    EXPECT_EQ(response.responseCode, ResponseCode::LEFT_ROOM_SUCCESS);

    it = recipients.find(player3);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player1);
    EXPECT_FALSE(it == recipients.end());


    response = controller.leaveRoom(player3);
    recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 2);
    EXPECT_EQ(response.responseCode, ResponseCode::LEFT_ROOM_SUCCESS);

    it = recipients.find(player3);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player1);
    EXPECT_FALSE(it == recipients.end());
}

TEST_F(ControllerTests, allPlayersLeaveRoom) {
    auto response = controller.joinRoom(inviteCode, player2);
    auto recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 2);
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);

    auto it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());


    response = controller.joinRoom(inviteCode, player3);
    recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 3);
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);

    it = recipients.find(player3);
    EXPECT_FALSE(it == recipients.end());


    response = controller.leaveRoom(player1);
    recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 3);
    EXPECT_EQ(response.responseCode, ResponseCode::LEFT_ROOM_SUCCESS);

    it = recipients.find(player3);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player1);
    EXPECT_FALSE(it == recipients.end());


    response = controller.leaveRoom(player2);
    recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 2);
    EXPECT_EQ(response.responseCode, ResponseCode::LEFT_ROOM_SUCCESS);

    it = recipients.find(player3);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());


    response = controller.leaveRoom(player3);
    recipients = response.recipientList;
    EXPECT_EQ(response.responseCode, ResponseCode::LEFT_ROOM_SUCCESS);

    EXPECT_EQ(recipients.size(), 1);
    it = recipients.find(player3);
    EXPECT_FALSE(it == recipients.end());
}

TEST_F(ControllerTests, leaveRoomTwice) {
    auto response = controller.joinRoom(inviteCode, player2);
    auto recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 2);
    EXPECT_EQ(response.responseCode, ResponseCode::JOIN_GAME_SUCCESS);

    auto it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());


    response = controller.leaveRoom(player2);
    recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 2);
    EXPECT_EQ(response.responseCode, ResponseCode::LEFT_ROOM_SUCCESS);

    it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());

    it = recipients.find(player1);
    EXPECT_FALSE(it == recipients.end());


    response = controller.leaveRoom(player2);
    recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 1);
    EXPECT_EQ(response.responseCode, ResponseCode::LEFT_ROOM_FAIL);

    it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());
}

TEST_F(ControllerTests, leaveRoomButNeverJoin) {
    auto response = controller.leaveRoom(player2);
    auto recipients = response.recipientList;
    EXPECT_EQ(recipients.size(), 1);
    EXPECT_EQ(response.responseCode, ResponseCode::LEFT_ROOM_FAIL);

    auto it = recipients.find(player2);
    EXPECT_FALSE(it == recipients.end());
}