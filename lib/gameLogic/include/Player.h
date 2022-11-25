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
        int id;
        playerTypeEnum playerType;
        std::string name; 
        int highScore;
        int currentScore;
        static int currentID;

    public:
        uintptr_t connectionID;

        Player(playerTypeEnum type) 
            :id{0}, playerType{type}
        { 
            name = "player" + id;
        }

        Player(playerTypeEnum type, std::string username) 
            :id{0}, playerType{type}, name{username}
        { }

        Player(playerTypeEnum type, networking::Connection& connection) 
            :id{0}, playerType{type}, connectionID{connection.id}
        { }

        Player(playerTypeEnum type, uintptr_t connectionID) 
            :id{0}, playerType{type}, connectionID{connectionID}
        { }
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
};


#endif //SOCIAL_GAMING_PLAYER_H
