//
// Created by igor on 05.10.16.
//

#include "Player.h"

int Player::getId() const {
    return _id;
}

const char * Player::getName() const {
    return _name;
}

int Player::getScore() const {
    return _score;
}


void Player::addScore(int _score) {
    _score += _score;
}


int Player::getShotsCount() const {
    return _shotsCount;
}

void Player::addShotsCount() {
    _shotsCount += 1;
}


Player::Player(int id, const char *name) {
    _id = id;
    _name = name;
    _score = 0;
    _shotsCount = 0;
}
