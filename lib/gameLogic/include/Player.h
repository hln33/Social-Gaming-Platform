//
// Created by mcgir on 10/6/2022.
//

#ifndef SOCIAL_GAMING_PLAYER_H
#define SOCIAL_GAMING_PLAYER_H
#include <string>
using namespace std;
class Player {
    private:
        string name;
        int id;
        int highScore;
        int currentScore;
        bool audience;

    public:
        Player(string name, bool audience, int highScore, int currentScore);
        Player(string name, bool audience, int currentScore);
        Player(string name, bool audience);

        Player(Player& other);
        string getName();
        void setName(string name);

        int getHighScore();
        void setHighScore(int highScore);

        int getCurrentScore();
        void setCurrentScore(int currentScore);

        int getId();
        void setId(int id);

        bool getAudience();
        void setAudience(bool audience);
};


#endif //SOCIAL_GAMING_PLAYER_H
