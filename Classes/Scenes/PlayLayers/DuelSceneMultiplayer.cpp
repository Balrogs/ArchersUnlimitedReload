//
// Created by igor on 17.11.16.
//

#include <GameEngine/Global/Misc/SocketClient.h>
#include "DuelSceneMultiplayer.h"

USING_NS_CC;

void DuelSceneMultiplayer::initWorld() {
    DuelScene2P::initWorld();
}

bool DuelSceneMultiplayer::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return DuelScene2P::_touchHandlerBegin(touch, event);
}

void DuelSceneMultiplayer::makeTurn(int id) {
    DuelScene2P::makeTurn(id);
}
