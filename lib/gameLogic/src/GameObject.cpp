//
// Created by mcgir on 10/14/2022.
//

#include "../include/GameObject.h"
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

std::string_view GameObject::getPrefix() {
    return prefix;
}

void GameObject::setPrefix(std::string_view prefix) {
    prefix = prefix;
}

std::string_view GameObject::getSuffix() {
    return suffix;
}

void GameObject::setSuffix(std::string_view suffix) {
    suffix = suffix;
}


std::string_view return_value(std::string_view value) {
    return value;
}




