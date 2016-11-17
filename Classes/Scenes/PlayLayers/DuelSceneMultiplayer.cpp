//
// Created by igor on 17.11.16.
//

#include "DuelSceneMultiplayer.h"


void DuelSceneMultiplayer::initWorld() {
    DuelScene2P::initWorld();
}

bool DuelSceneMultiplayer::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return DuelScene2P::_touchHandlerBegin(touch, event);
}

void DuelSceneMultiplayer::makeTurn(int id) {
    DuelScene2P::makeTurn(id);
}
