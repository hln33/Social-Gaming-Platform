//
// Created by mcgir on 10/14/2022.
//

#ifndef INC_373PROJECT_GAMEOBJECT_H
#define INC_373PROJECT_GAMEOBJECT_H

#include <string>

class GameObject {
    std::string_view name;
    std::string_view prefix;
    std::string_view suffix;
    public:
        GameObject(std::string_view name);
        std::string_view getName();
        void setName(std::string_view name);
        std::string_view getPrefix();
        void setPrefix(std::string_view prefix);
        std::string_view getSuffix();
        void setSuffix(std::string_view suffix);
        std::string_view return_value(std::string_view value);
};


#endif //INC_373PROJECT_GAMEOBJECT_H
