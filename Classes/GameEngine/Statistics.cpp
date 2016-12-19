#include "Statistics.h"

Statistics *Statistics::create(int type) {
    Statistics *ret = new(std::nothrow) Statistics();
    if (!ret || !ret->init(type)) {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool Statistics::init(int type) {

    _view = Node::create();

    _type = type;
    _coinsGained = 0;
    _level = 0;
    _hp = 100;
    _timeSpent = 0;

    return true;
}

int Statistics::getLevel() {
    return _level;
}

void Statistics::increaseLevel(int level) {
    Statistics::_level = level;
}

int Statistics::getCoinsGained() {
    return _coinsGained;
}

void Statistics::increaseCoins(int diff) {
    Statistics::_coinsGained += diff;
}

long Statistics::getTimeSpent() {
    return _timeSpent;
}

void Statistics::setTime(long timeSpent) {
    Statistics::_timeSpent = timeSpent;
}

float Statistics::getHp() {
    return _hp;
}

void Statistics::setHp(float hp) {
    Statistics::_hp = hp;
}

int Statistics::getType() {
    return _type;
}
