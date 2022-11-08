//
// Created by mcgir on 10/6/2022.
//

#ifndef SOCIAL_GAMING_PLAYER_H
#define SOCIAL_GAMING_PLAYER_H

#include <string>

enum playerTypeEnum{
    host,
    player,
    audience
};

class Player {
    private:
        std::string_view name;
        int id;
        int highScore;
        int currentScore;
        playerTypeEnum playerType;

        static int currentID;

    public:
        Player(playerTypeEnum playerType);
        // Player(std::string_view name, bool audience, int highScore, int currentScore);
        // Player(std::string_view name, bool audience, int currentScore);
        // Player(std::string_view name, bool audience);

        std::string_view getName();
        void setName(std::string_view name);

        // int getHighScore();
        // void setHighScore(int highScore);

        // int getCurrentScore();
        // void setCurrentScore(int currentScore);

        int getId();
        void setId(int id);

        playerTypeEnum getplayerType();
        void setplayerType(playerTypeEnum type);

        void message(std::string_view message);
};


#endif //SOCIAL_GAMING_PLAYER_H
