//
// Created by mcgir on 10/6/2022.
//

#ifndef SOCIAL_GAMING_PLAYER_H
#define SOCIAL_GAMING_PLAYER_H

#include <string>
#include <string_view>
#include "Server.h"

enum playerTypeEnum{
    host,
    player,
    audience
};

class Player {
    private:
        std::string name; 
        int id;
        int highScore;
        int currentScore;
        playerTypeEnum playerType;
        networking::Connection connection;
        static int currentID;

    public:
        Player(playerTypeEnum playerType);
        Player(playerTypeEnum playerType, std::string username);
        Player(playerTypeEnum playerType, networking::Connection connection);
        // Player(std::string_view name, bool audience, int highScore, int currentScore);
        // Player(std::string_view name, bool audience, int currentScore);
        // Player(std::string_view name, bool audience);

        std::string getName();
        void setName(std::string name);

        // int getHighScore();
        // void setHighScore(int highScore);

        // int getCurrentScore();
        // void setCurrentScore(int currentScore);

        int getId();
        void setId(int id);

        bool getAudience() {
            return this->playerType == playerTypeEnum::audience;
        }

        playerTypeEnum getplayerType();
        void setplayerType(playerTypeEnum type);

        void message(std::string message);

        void setConnection(networking::Connection connection);
        networking::Connection getConnection();
};


#endif //SOCIAL_GAMING_PLAYER_H
