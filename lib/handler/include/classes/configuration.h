#pragma once

#include <string>

class Configuration {
    public:
        std::string gameName;
        int minPlayers;
        int maxPlayers;
        bool audienceAllowed;
        std::string kind;
        std::string prompt;

        Configuration(std::string gameName, int minPlayers, int maxPlayers, bool, std::string kind, std::string prompt);
};