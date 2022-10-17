//
// Created by mcgir on 10/14/2022.
//

#ifndef INC_373PROJECT_GAMEOBJECT_H
#define INC_373PROJECT_GAMEOBJECT_H

#include<string>
using namespace std;

class GameObject {
    string name;
    // GameObject beats;
    public:
        GameObject(string name);
        GameObject(GameObject& other);

        void setBeats(GameObject beats);
        GameObject getBeats();

        string getName();
        void setName(string name);
};


#endif //INC_373PROJECT_GAMEOBJECT_H
