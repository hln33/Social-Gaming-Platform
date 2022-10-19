//
// Created by mcgir on 10/6/2022.
//

#include "../includes/Player.h"
#include <iostream>
#include <cstdlib>
#include <vector>

Player::Player(String name, bool audience, int highScore, int currentScore) {
    name = name;
    audience = audience;
    highScore = highScore;
    currentScore = currentScore;
}

Player::Player(String name, bool audience, int currentScore) {
    name = name;
    audience = audience;
    currentScore = currentScore;
}

Player::Player(String name, bool audience) {
    name = name;
    audience = audience;
}

Player::Player(&Player other) {
    name = other.name;
    audience = other.audience;
    highScore = other.highScore;
    currentScore = other.currentScore;
}

int Player::getName() {
    return name;
}

void Player::setName(int name) {
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
