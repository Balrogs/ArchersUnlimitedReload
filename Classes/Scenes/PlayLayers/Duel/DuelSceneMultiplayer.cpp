#include <GameEngine/Global/Misc/SocketClient.h>
#include <GameEngine/Objects/Environment/Ground.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include <GameEngine/Global/Variables.h>
#include "DuelSceneMultiplayer.h"

USING_NS_CC;

DuelSceneMultiplayer *DuelSceneMultiplayer::create(Statistics *stats) {
    DuelSceneMultiplayer *ret = new(std::nothrow) DuelSceneMultiplayer();
    if (ret && ret->init(stats)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    _instance = ret;

    return ret;
}

void DuelSceneMultiplayer::initWorld() {
    _isStarted = false;

    _client = SocketClient::getInstance();

}

bool DuelSceneMultiplayer::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_isStarted && this->_turnId == _client->getDBPlayer()->getId()) {
        return BattleParent::_touchHandlerBegin(touch, event);
    } else
        return false;
}

void DuelSceneMultiplayer::makeTurn(int id) {
    if (id == -1 || this->_turnId == 0) {
        this->_turnId = id;
        return;
    }

    if (this->_turnId != id) {
        float delay = 2.f;
        if (this->getPosition().x >= visibleSize.width / 2) {
            delay = 0.5f;
        }
        auto player = getHero(id);
        auto action = Sequence::create(
                Spawn::createWithTwoActions(
                        MoveTo::create(delay, Vec2(-_player->getPosition().x + visibleSize.width / 2, 0.f)),
                        CallFunc::create(
                                [&, delay]() {
                                    auto vec = Vec2(-_player->getPosition().x +
                                                    visibleSize.width / 2, 0.f);
                                    this->_bg->runAction(MoveTo::create(delay, vec));
                                }
                        )),
                CallFunc::create(
                        [&]() {
                            if (player->getPlayer()->getId() == _player->getPlayer()->getId())
                                UI::enableArrows(player, true);
                            this->_turnId = player->getPlayer()->getId();
                        }
                ),
                NULL
        );
        this->runAction(action);
    }
}

void DuelSceneMultiplayer::setPlayer(int id) {
    MultiplayerBattle::setPlayer(id);
    _ui->initDuel(visibleSize, _hero1, _hero2);
}
