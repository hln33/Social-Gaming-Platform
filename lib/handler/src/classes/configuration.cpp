#include "configuration.h"

Configuration::Configuration(std::string name, int min, int max, bool audience, std::string kind, std::string prompt) 
            : 
            gameName{name},
            minPlayers(min),
            maxPlayers(max),
            audienceAllowed(audience),
            kind(kind),
            prompt(prompt)
        {}