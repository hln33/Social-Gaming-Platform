#include <cctype>
#include <vector>
#include <algorithm>


#include "room.h"
#include "player_storage.h"

class IRoomConfig {
public:
    virtual void setContext(Room&);

    virtual bool allow(IPlayer&) const;
};

class Policy {

};


class PolicyHolder {
public:
    void addPolicy(Policy& policy) const {
        this->policies.push_back(policy);
    }

    bool applyAllTheJoinPolicies(IPlayer& player) {
        return std::all_of(
            this->policies.begin(), 
            this->policies.end(), 
            [&player](auto x) {
                return x->allow(player);
            }
        );
    }

    bool applyAllTheStartPolicies() {
        return std::all_of(
            this->policies.begin(), 
            this->policies.end(), 
            [](auto x) {
                return x.allow();
            }
        );
    }

public:
    std::vector<Policy> policies;
};

// the join policies will be applied whenever somone tries to
// join the game room. It will decide if it should allow that
// player to join
class JoinPolicy_PlayerLimitsConfigOpt : public IRoomConfig {
public:
    PlayerLimitsConfigOpt(size_t max_players, IRoom* room) : 
        maxPlayers{max_players},
        roomPointer{room}
    { }

    bool allow(IPlayer& player) const override {
        return this->roomPointer->getNumPlayers() < this->maxPlayers;
    }

private:
    size_t maxPlayers;
    IRoom* roomPointer;
};

// the start policies will be applicd whenever the game is
// started. It will decide if the room meets the requirements
// set by the creator
class StartPolicy_Player;

