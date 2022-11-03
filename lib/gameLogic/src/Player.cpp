//
// Created by mcgir on 10/6/2022.
//

#include "../includes/Player.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

Player::Player(std::string_view name, bool audience, int highScore, int currentScore) {
    name = name;
    audience = audience;
    highScore = highScore;
    currentScore = currentScore;
}

Player::Player(std::string_view name, bool audience, int currentScore) {
    name = name;
    audience = audience;
    currentScore = currentScore;
}

Player::Player(std::string_view name, bool audience) {
    name = name;
    audience = audience;
}

std::string_view Player::getName() {
    return name;
}

void Player::setName(std::string_view name) {
    name = name;
}

int Player::getHighScore() {
    return highScore;
}

void Player::setHighScore(int highScore) {
    highScore = highScore;
}

int Player::getCurrentScore() {
    return currentScore;
}

void Player::setCurrentScore(int currentScore) {
    currentScore = currentScore;
}

int Player::getId() {
    return id;
}

void Player::setId(int id) {
    //shouldn't be used as each user should have a unique id
    id = id;
}

bool Player::getAudience() {
    return audience;
}

void Player::setAudience(bool audience) {
    audience = audience;
}

void Player::message(std::string_view message) {
    std::cout << message << std::endl;
}
