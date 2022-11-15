#pragma once

#include <cctype>
#include <vector>
#include <memory>
#include <algorithm>
#include <string> 

#include "Player.h"
// #include "std::vector<Player>.h"

class JoinPolicyInterface {
public:
    virtual bool allow(Player&, std::vector<Player> const&) const = 0;
    virtual std::string getPolicyName() const = 0;
};

// the join policies will be applied whenever somone tries to
// join the game room. It will decide if it should allow that
// player to join
class MaxPlayersOpt : public JoinPolicyInterface {
public:
    MaxPlayersOpt(size_t max_players) : 
        maxPlayers{max_players}
    { }

    bool allow(Player& player, std::vector<Player> const& members) const override {
        return members.size() < this->maxPlayers;
    }

    std::string getPolicyName() const override {
        return this->policyName;
    }

private:
    size_t maxPlayers;
    std::string policyName = "max players policy";
};

class AudienceOpt : public JoinPolicyInterface {
public:
    AudienceOpt(bool allows_audience) : 
        allowsAudience{allows_audience}
    { }

    bool allow(Player& player, std::vector<Player> const& members) const override {
        return player.getAudience() == this->allowsAudience;
    }

    std::string getPolicyName() const override {
        return this->policyName;
    }

private:
    bool allowsAudience;
    std::string policyName = "audience policy";
};

// the start policies will be applicd whenever the game is
// started. It will decide if the room meets the requirements
// set by the creator
class StartPolicyInterface {
public:
    virtual bool allow(std::vector<Player> const&) const = 0;
    virtual std::string getPolicyName() const = 0;
};


class MinPlayersOpt : public StartPolicyInterface {
public:
    MinPlayersOpt(size_t min_players) : 
        minPlayers{min_players}
    { }

    bool allow(std::vector<Player> const& members) const override {
        return members.size() >= this->minPlayers;
    }

    std::string getPolicyName() const override {
        return this->policyName;
    }

private:
    std::string policyName = "max players policy";
    size_t minPlayers;
};

