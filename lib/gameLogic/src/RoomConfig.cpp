#include <memory>
#include <vector>

#include "RoomPolicy.h"
#include "RoomConfig.h"

void buildRoomConfig(RoomConfig& config, RoomConfigBuilderOptions& buildOptions, std::vector<Player>& players) {
    // build join policies
    auto maxPlayerPolicy = std::make_unique<MaxPlayersOpt>(MaxPlayersOpt{buildOptions.maxPlayers, players});
    auto audiencePolicy = std::make_unique<AudienceOpt>(AudienceOpt{buildOptions.allowAudience});

    config.addJoinPolicy(std::move(maxPlayerPolicy));
    config.addJoinPolicy(std::move(audiencePolicy));

    // build start policies
    auto minPlayerPolicy = std::make_unique<MinPlayersOpt>(MinPlayersOpt{buildOptions.minPlayers, players});

    config.addStartPolicy(std::move(minPlayerPolicy));
}
