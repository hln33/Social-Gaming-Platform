//
// Created by mcgir on 10/14/2022.
//

#ifndef INC_373PROJECT_GAMEOBJECT_H
#define INC_373PROJECT_GAMEOBJECT_H


class GameObject {
    String name;
    GameObject beats;
    public:
        GameObject(String name);
        GameObject(&GameObject other);

        void setBeats(GameObject beats);
        GameObject getBeats();

        String getName();
        void setName(String name);
};


#endif //INC_373PROJECT_GAMEOBJECT_H
