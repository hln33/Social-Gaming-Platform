//
// Created by mcgir on 10/14/2022.
//

#include "includes/GameObject.h"

GameObject::GameObject(String name) {
    name = name;
}

GameObject::GameObject(&GameObject other) {
    name = other.name;
}

GameObject::getName() {
    return name;
}

GameObject::setName(String name) {
    name = name;
}

GameObject::getBeats() {
    return beats;
}

GameObject::setBeats(GameObject beats) {
    beats = beats;
}