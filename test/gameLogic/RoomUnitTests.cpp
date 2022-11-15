#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <memory>
#include <exception>

#include "Room.h"
#include "ConfigDecorator.h"

class PlayerStub : public IPlayer {
public:
    void setPublicId(IUniqueId) override {
        return;
    }

    IUniqueId getPublicId() const override {
        return UniqueId("fake-id");
    }
};

PlayerStub fakeplayer = PlayerStub{};

class PlayerStorageStub : public IPlayerStorage {
public:
    void addPlayerRecord(IPlayer& player) override {
        this->records++;
        return;
    }
    void updatePlayerRecord(IPlayer& player) {
        return;
    }
    void removePlayerRecord(IUniqueId& uid) override {
        this->records--;
        return;
    }
    const IPlayer& getPlayerRecord(IUniqueId& uid) const override {
        return fakeplayer;
    }
    int getNumPlayerRecords() const override {
        return this->records;
    }

private:
    int records = 0;
};

class RoomConfigStub : public IRoomConfig {
public:
    void setContext(std::unique_ptr<IRoom> room) override {
        return;
    }
    bool allow(IPlayer& player) const override {
        return true;
    }
};

class RoomConfigNeverAllowStub : public IRoomConfig {
public:
    void setContext(std::unique_ptr<IRoom> room) override {
        return;
    }
    bool allow(IPlayer& player) const override {
        return false;
    }
};


// get # of players
TEST(RoomTests, HandlesNumPlayersEqualsZero) {
    Room room{
        std::make_unique<RoomConfigStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    EXPECT_EQ(0, room.getNumPlayers());
}


TEST(RoomTests, HandlesNumPlayersMoreThanZero) {
    Room room{
        std::make_unique<RoomConfigStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    auto p = PlayerStub{};
    room.addPlayer(p);

    EXPECT_EQ(1, room.getNumPlayers());
}

TEST(RoomTests, HandlesManyManyPlayers) {
    Room room{
        std::make_unique<RoomConfigStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    for (int i = 0; i < 1000; i++) {
        auto p = PlayerStub{};
        room.addPlayer(p);
    }

    EXPECT_EQ(1000, room.getNumPlayers());
}


// add player
TEST(RoomTests, HandlesJoining) {
    Room room{
        std::make_unique<RoomConfigStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    auto p = PlayerStub{};
    EXPECT_EQ(0, room.getNumPlayers());
    EXPECT_TRUE( room.addPlayer(p) );
    EXPECT_EQ(1, room.getNumPlayers());
}

TEST(RoomTests, HandlesJoiningWithPolicy) {
    Room room{
        std::make_unique<RoomConfigNeverAllowStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    auto p = PlayerStub{};
    EXPECT_EQ(0, room.getNumPlayers());
    EXPECT_FALSE( room.addPlayer(p) );
    EXPECT_EQ(0, room.getNumPlayers());
}

// remove player
TEST(RoomTests, HandlesLeaving) {
    Room room{
        std::make_unique<RoomConfigStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    IPlayer player = PlayerStub{};
    room.addPlayer(player);

    EXPECT_EQ(1, room.getNumPlayers());

    room.removePlayer(player);

    EXPECT_EQ(0, room.getNumPlayers());
}

// multiple adds/removes
TEST(RoomTests, HandlesAddsAndRemoves) {
    Room room{
        std::make_unique<RoomConfigStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    for (int i = 0; i < 100; i++) {
        IPlayer player = PlayerStub{};
        room.addPlayer(player);
        room.removePlayer(player);
    }

    EXPECT_EQ(0, room.getNumPlayers());
}

// #get player 
TEST(RoomTests, HandlesGetPlayerWhenEmpty) {
    Room room{
        std::make_unique<RoomConfigStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    EXPECT_EQ(0, room.getNumPlayers());

    bool wasOk = false;
    auto id = UniqueId{"fake-id"};
    try {
        room.getPlayer(id);
        wasOk = true;
    } catch (std::exception&) { }

    EXPECT_FALSE(wasOk);
}


TEST(RoomTests, HandlesGetPlayerWrongID) {
    Room room{
        std::make_unique<RoomConfigStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    auto p = PlayerStub{};
    room.addPlayer(p);

    EXPECT_EQ(1, room.getNumPlayers());

    bool wasOk = false;
    auto id = UniqueId{"wrong-id"};
    try {
        room.getPlayer(id);
        wasOk = true;
    } catch (std::exception&) { }

    EXPECT_FALSE(wasOk);
}

TEST(RoomTests, HandlesGetPlayerRightID) {
    Room room{
        std::make_unique<RoomConfigStub>(),
        std::make_unique<PlayerStorageStub>()
    };

    auto p = PlayerStub{};
    room.addPlayer(p);

    EXPECT_EQ(1, room.getNumPlayers());


    bool wasOk = false;
    auto id = UniqueId{"fake-id"};
    try {
        room.getPlayer(id);
        wasOk = true;
    } catch (std::exception&) { }

    EXPECT_TRUE(wasOk);
}
