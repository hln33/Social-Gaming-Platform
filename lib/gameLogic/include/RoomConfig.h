#pragma once

#include <cctype>
#include <vector>
#include <memory>
#include <algorithm>
#include <string_view>

#include "RoomPolicy.h"
#include "PlayerStorage.h"

class RoomConfig { 
public:
    std::string result;

    void addJoinPolicy(std::unique_ptr<JoinPolicyInterface> joinPolicy) {
        joinPolicies.push_back(std::move(joinPolicy));
    }

    void addStartPolicy(std::unique_ptr<StartPolicyInterface> startPolicy) {
        startPolicies.push_back(std::move(startPolicy));
    }

    bool satisfiesJoinPolicies(Player& player, std::vector<Player> const& members) {
        auto policy = std::find_if(
            joinPolicies.begin(), 
            joinPolicies.end(), 
            [&player, &members](std::unique_ptr<JoinPolicyInterface> const& x) {
                return !x->allow(player, members);
            }
        );

        bool allowed = policy == joinPolicies.end();
        if (!allowed) {
            this->result = policy->get()->getPolicyName();
        }

        return allowed;
    }

    bool satisfiesStartPolicies(std::vector<Player> const& members) {
        auto policy = std::find_if(
            startPolicies.begin(),
            startPolicies.end(),
            [&members](std::unique_ptr<StartPolicyInterface> const& x) {
                return !x->allow(members);
            }
        );

        bool allowed = policy == startPolicies.end();
        if (!allowed) {
            this->result = policy->get()->getPolicyName();
        }

        return allowed;
    }

private:

    std::vector<std::unique_ptr<JoinPolicyInterface>> joinPolicies;
    std::vector<std::unique_ptr<StartPolicyInterface>> startPolicies;
};

struct RoomConfigBuilderOptions {
    std::string name;
    size_t maxPlayers;
    size_t minPlayers;
    bool allowAudience;
};

void buildRoomConfig(RoomConfig& config, RoomConfigBuilderOptions& buildOptions, std::vector<Player>& players);

