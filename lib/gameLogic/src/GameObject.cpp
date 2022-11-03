//
// Created by mcgir on 10/14/2022.
//

#include "../includes/GameObject.h"
#include <string>

GameObject::GameObject(std::string_view name) {
    name = name;
}

std::string_view GameObject::getName() {
    return name;
}

void GameObject::setName(std::string_view name) {
    name = name;
}
