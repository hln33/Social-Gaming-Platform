//
// Created by mcgir on 10/14/2022.
//

#include "../includes/GameObject.h"


GameObject::GameObject(std::string name) {
    name = name;
    beats = nullptr;
}

std::string GameObject::getName() {
    return name;
}

void GameObject::setName(std::string name) {
    name = name;
}

GameObject GameObject::getBeats() {
    return *beats;
}

void GameObject::setBeats(GameObject* beats) {
    this->beats = beats;
}