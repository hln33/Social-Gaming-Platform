#include <memory>

#include "RoomPolicy.h"
#include "RoomConfig.h"

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
