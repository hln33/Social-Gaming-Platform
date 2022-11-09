#include <gtest/gtest.h>
#include <memory>
#include <exception>

#include "Player.h"

TEST(Player, PlayerHasUniqueId) {
    Player player;
    EXPECT_TRUE(player.getPublicId().isValid());
}

TEST(Player, PlayerHasID) {
    Player player;
    EXPECT_TRUE(player.getId() != 0);
}

TEST(Player, PlayerHasName) {
    Player player;
    EXPECT_TRUE(player.getName().length() > 0);
}

TEST(Player, PlayerHasRoom) {
    Player player;
    EXPECT_TRUE(player.getRoom() != nullptr);
}

TEST(Player, PlayerHasCurrentScore) {
    Player player;
    EXPECT_TRUE(player.getCurrentScore() >= 0);
}

TEST(Player, PlayerHasHighScore) {
    Player player;
    EXPECT_TRUE(player.getHighScore() >= 0);
}

TEST(Player, PlayerHasAudienceDefault) {
    Player player;
    EXPECT_TRUE(player.getAudience());
}

TEST(Player, PlayerHasAudienceSet) {
    Player player;
    player.setAudience(false);
    ASSERT_FALSE(player.getAudience());
}

TEST(Player, PlayerHasNameSet) {
    Player player;
    player.setName("test");
    EXPECT_TRUE(player.getName() == "test");
}

TEST(Player, PlayerGetName) {
    Player player;
    player.setName("test");
    EXPECT_TRUE(player.getName() == "test");
}

TEST(Player, PlayerHasHighScoreSet) {
    Player player;
    player.setHighScore(100);
    EXPECT_TRUE(player.getHighScore() == 100);
}

TEST(Player, PlayerHasCurrentScoreSet) {
    Player player;
    player.setCurrentScore(100);
    EXPECT_TRUE(player.getCurrentScore() == 100);
}

TEST(Player, PlayerHasHighScoreGet) {
    Player player;
    player.setHighScore(100);
    EXPECT_TRUE(player.getHighScore() == 100);
}

TEST(Player, PlayerHasCurrentScoreGet) {
    Player player;
    player.setCurrentScore(100);
    EXPECT_TRUE(player.getCurrentScore() == 100);
}

TEST(Player, PlayerHasIdSet) {
    Player player;
    player.setId(100);
    EXPECT_TRUE(player.getId() == 100);
}

TEST(Player, PlayerHasIdGet) {
    Player player;
    player.setId(100);
    EXPECT_TRUE(player.getId() == 100);
}

TEST(Player, PlayerHasAudienceGet) {
    Player player;
    player.setAudience(false);
    ASSERT_FALSE(player.getAudience());
}

TEST(Player, PlayerHasAudienceSet) {
    Player player;
    player.setAudience(false);
    ASSERT_FALSE(player.getAudience());
}

