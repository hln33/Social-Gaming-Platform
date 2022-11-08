#pragma once

#include <cctype>
#include <vector>
#include <memory>
#include <algorithm>

#include "RoomPolicy.h"
#include "Player.h"
#include "PlayerStorage.h"

class RoomConfig {
public:
    void addJoinPolicy(std::unique_ptr<JoinPolicyInterface> joinPolicy) {
        joinPolicies.push_back(std::move(joinPolicy));
    }

    void addStartPolicy(std::unique_ptr<StartPolicyInterface> startPolicy) {
        startPolicies.push_back(std::move(startPolicy));
    }

    bool satisfiesJoinPolicies(Player& player) const {
        return std::all_of(
            joinPolicies.begin(), 
            joinPolicies.end(), 
            [&player](std::unique_ptr<JoinPolicyInterface> const& x) {
                return x->allow(player);
            }
        );
    }

    bool satisfiesStartPolicies() const {
        return std::all_of(
            startPolicies.begin(),
            startPolicies.end(),
            [](std::unique_ptr<StartPolicyInterface> const& x) {
                return x->allow();
            }
        );
    }

private:
    std::vector<std::unique_ptr<JoinPolicyInterface>> joinPolicies;
    std::vector<std::unique_ptr<StartPolicyInterface>> startPolicies;
};



struct RoomConfigBuilderOptions {
    size_t maxPlayers;
    size_t minPlayers;
    bool allowAudience;
};

RoomConfig buildRoomConfig(RoomConfigBuilderOptions& buildOptions, PlayerStorage& playersRef) {
    RoomConfig config = RoomConfig{};

    // build join policies
    auto maxPlayerPolicy = std::make_unique<MaxPlayersOpt>(MaxPlayersOpt{buildOptions.maxPlayers, playersRef});
    auto audiencePolicy = std::make_unique<AudienceOpt>(AudienceOpt{buildOptions.allowAudience});

    config.addJoinPolicy(std::move(maxPlayerPolicy));
    config.addJoinPolicy(std::move(audiencePolicy));

    // build start policies
    auto minPlayerPolicy = std::make_unique<MinPlayersOpt>(MinPlayersOpt{buildOptions.minPlayers, playersRef});

    config.addStartPolicy(std::move(minPlayerPolicy));

    return config;
}

