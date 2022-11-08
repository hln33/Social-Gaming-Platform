
#include <string>

#include "PlayerStorage.h"
#include "UniqueId.h"


class PlayerStub : public IPlayer {
public:
    void setPublicId(IUniqueId) override {
        return;
    }

    void setTestPublicId(int id) {
        this->id = id;
    }

    IUniqueId getPublicId() const override {
        return UniqueId(std::to_string(this->id));
    }
private:
    int id = 0;
};

// get # of players
TEST(PlayerStorageTests, SanityCheck) {
    PlayerStorage storage{};

    EXPECT_EQ(0, storage.getNumPlayerRecords());
}

TEST(PlayerStorageTests, HandlesAddOnePlayer) {
    PlayerStorage storage{};

    PlayerStub* player = new PlayerStub{};
    storage.addPlayerRecord(player);

    EXPECT_EQ(1, storage.getNumPlayerRecords());
}

TEST(PlayerStorageTests, HandlesManyAddsWithSameId) {
    PlayerStorage storage{};

    for (int i = 0; i < 1000; i++) {
        PlayerStub* player = new PlayerStub{};
        storage.addPlayerRecord(player);
    }

    EXPECT_EQ(1, storage.getNumPlayerRecords());
}

TEST(PlayerStorageTests, HandlesManyAddsWithDifferentIds) {
    PlayerStorage storage{};

    for (int i = 0; i < 1000; i++) {
        PlayerStub* player = new PlayerStub{};
        player.setTestPublicId(i);
        storage.addPlayerRecord(player);
    }

    EXPECT_EQ(1000, storage.getNumPlayerRecords());
}

// test update player record
TEST(PlayerStorageTests, HandlesUpdatePlayerRecord) {
    PlayerStorage storage{};

    PlayerStub* player = new PlayerStub{};
    player.setTestPublicId(0);
    storage.addPlayerRecord(player);

    EXPECT_EQ(1, storage.getNumPlayerRecords());

    UniqueId id{"0"};
    EXPECT_EQ("0", storage.getPlayerRecord(id)->getPublicId().getValue());
}

