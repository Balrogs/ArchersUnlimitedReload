#include "AppleMultiplayer.h"

AppleMultiplayer *AppleMultiplayer::create(Statistics *stats) {
    AppleMultiplayer *ret = new(std::nothrow) AppleMultiplayer();

    if (ret && ret->init(stats)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    _instance = ret;

    return ret;
}

void AppleMultiplayer::_nextLevelAction() {

}

void AppleMultiplayer::addApple() {

}

void AppleMultiplayer::initWorld() {

}

void AppleMultiplayer::initObjects() {

}

bool AppleMultiplayer::isGameOver() {
    return DuelScene::isGameOver();
}
