//
// Created by igor on 17.11.16.
//

#include <GameEngine/Global/SocketClient.h>
#include "DuelSceneMultiplayer.h"

USING_NS_CC;

void DuelSceneMultiplayer::initWorld() {
    DuelScene2P::initWorld();
    client = new SocketClient();
}

bool DuelSceneMultiplayer::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return DuelScene2P::_touchHandlerBegin(touch, event);
}

void DuelSceneMultiplayer::makeTurn(int id) {
    DuelScene2P::makeTurn(id);
    client->send_data("{\"name\":\"some other username\",\"country\":1,\"password\":\"1234\",\"code\":2}");
    CCLOG(client->receive(512).c_str());
}
