//
// Created by mcgir on 10/6/2022.
//

#ifndef SOCIAL_GAMING_PLAYER_H
#define SOCIAL_GAMING_PLAYER_H


class Player {
    private:
        String name;
        int id;
        int highScore;
        int currentScore;
        bool audience;

    public:
        Player(String name, bool audience, int highScore, int currentScore);
        Player(String name, bool audience, int currentScore);
        Player(String name, bool audience);

        Player(&Player other);
        String getName();
        void setName(String name);

        int getHighScore();
        void setHighScore(int highScore);

        int getCurrentScore();
        void setCurrentScore(int currentScore);

        int getId();
        void setId(int id);

        bool getAudience();
        void setAudience(bool audience);

        void message(String message);
};


#endif //SOCIAL_GAMING_PLAYER_H
