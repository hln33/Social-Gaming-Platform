#pragma once

#include <cctype>
#include <vector>
#include <memory>
#include <algorithm>

#include "Player.h"
#include "PlayerStorage.h"

class JoinPolicyInterface {
public:
    virtual bool allow(Player&) const = 0;
};

// the join policies will be applied whenever somone tries to
// join the game room. It will decide if it should allow that
// player to join
class MaxPlayersOpt : public JoinPolicyInterface {
public:
    MaxPlayersOpt(size_t max_players, PlayerStorage& players) : 
        maxPlayers{max_players},
        storageRef{players}
    { }

    bool allow(Player& player) const override {
        return this->storageRef.getNumPlayerRecords() < this->maxPlayers;
    }

private:
    size_t maxPlayers;
    PlayerStorage& storageRef;
};

class AudienceOpt : public JoinPolicyInterface {
public:
    AudienceOpt(bool allows_audience) : 
        allowsAudience{allows_audience}
    { }

    bool allow(Player& player) const override {
        return player.getAudience() == this->allowsAudience;
    }

private:
    bool allowsAudience;
};

// the start policies will be applicd whenever the game is
// started. It will decide if the room meets the requirements
// set by the creator
class StartPolicyInterface {
public:
    virtual bool allow() const = 0;
};


class MinPlayersOpt : public StartPolicyInterface {
public:
    MinPlayersOpt(size_t min_players, PlayerStorage& players) : 
        minPlayers{min_players},
        storageRef{players}
    { }

    bool allow() const override {
        return this->storageRef.getNumPlayerRecords() >= this->minPlayers;
    }

private:
    size_t minPlayers;
    PlayerStorage& storageRef;
};

