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

    public:
        Player(String name, int highScore, int currentScore);
        Player(String name, int currentScore);
        Player(String name);

        String getName();
        void setName(String name);

        int getHighScore();
        void setHighScore(int highScore);

        int getCurrentScore();
        void setCurrentScore(int currentScore);

        int getId();
        void setId(int id);
};


#endif //SOCIAL_GAMING_PLAYER_H
