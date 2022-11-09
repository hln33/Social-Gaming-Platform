#include <memory>
#include <vector>

#include "RoomPolicy.h"
#include "RoomConfig.h"

RoomConfig buildRoomConfig(RoomConfigBuilderOptions& buildOptions, std::vector<Player>& players) {
    RoomConfig config = RoomConfig{};

    // build join policies
    auto maxPlayerPolicy = std::make_unique<MaxPlayersOpt>(MaxPlayersOpt{buildOptions.maxPlayers, players});
    auto audiencePolicy = std::make_unique<AudienceOpt>(AudienceOpt{buildOptions.allowAudience});

    config.addJoinPolicy(std::move(maxPlayerPolicy));
    config.addJoinPolicy(std::move(audiencePolicy));

    // build start policies
    auto minPlayerPolicy = std::make_unique<MinPlayersOpt>(MinPlayersOpt{buildOptions.minPlayers, players});

    config.addStartPolicy(std::move(minPlayerPolicy));

    return config;
}
