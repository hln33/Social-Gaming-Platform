//
// Created by mcgir on 10/14/2022.
//

#ifndef INC_373PROJECT_GAMEOBJECT_H
#define INC_373PROJECT_GAMEOBJECT_H

#include <string>


class GameObject {
    std::string name;
    GameObject* beats;
    public:
        GameObject(std::string name);

        void setBeats(GameObject* beats);

        GameObject getBeats();

        std::string getName();
        void setName(std::string name);
        
};


#endif //INC_373PROJECT_GAMEOBJECT_H
