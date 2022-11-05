//
// Created by mcgir on 10/14/2022.
//

#ifndef INC_373PROJECT_GAMEOBJECT_H
#define INC_373PROJECT_GAMEOBJECT_H

#include <string>

class GameObject {
    std::string_view name;
    public:
        GameObject(std::string_view name);
        std::string_view getName();
        void setName(std::string_view name);
};


#endif //INC_373PROJECT_GAMEOBJECT_H
