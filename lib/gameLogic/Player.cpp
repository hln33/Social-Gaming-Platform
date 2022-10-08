//
// Created by mcgir on 10/6/2022.
//

#include "Player.h"
#include <iostream>
#include <cstdlib>

Player::Player(int name) {
    name = name;
    highScore = 0;
    currentScore = 0;
    id = rand();
}

Player::Player(int name, int currentScore) {
    name = name;
    highScore = 0;
    currentScore = currentScore;
    id = rand();
}

Player::Player(int name, int highScore, int currentScore) {
    name = name;
    highScore = highScore;
    currentScore = currentScore;
    id = rand();
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