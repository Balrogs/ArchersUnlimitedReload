//
// Created by igor on 17.11.16.
//

#include <GameEngine/Objects/Environment/Ground.h>
#include "DuelScene2P.h"

USING_NS_CC;

void DuelScene2P::initWorld() {

    Ground *ground = new Ground(GROUND, visibleSize.width * 4);
    this->addChild(ground);

    _hero1 = new DuelHero(visibleSize.width / 2, DuelScene::GROUND, "Player 1");

    _hero2 = new DuelHero(visibleSize.width * 3 - 150.f, DuelScene::GROUND, "Player 2");

    _hero2->changeFacedir(-1);
    _player1 = _hero1->getPlayer();
    _player2 = _hero2->getPlayer();
    _player2->setHAlignment(cocos2d::TextHAlignment::RIGHT);

    ui->initDuel(visibleSize, _hero1, _hero2);
    _player = _hero1;

}

bool DuelScene2P::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (this->_turnId == _player->getPlayer()->getId()) {
        return BattleScene::_touchHandlerBegin(touch, event);
    } else
        return false;
}

void DuelScene2P::makeTurn(int id) {
    if (id == -1 || this->_turnId == 0) {
        this->_turnId = id;
        return;
    }

    if (this->_turnId != id) {
        float delay = 2.f;
        if (this->getPosition().x >= visibleSize.width / 2) {
            delay = 0.5f;
        }
        _player = getHero(id);
        auto action = Sequence::create(
                MoveTo::create(delay, Vec2(-_player->getPosition().x + visibleSize.width / 2, 0.f)),
                CallFunc::create(
                        [&]() {
                            UI::enableArrows(_player, true);
                            this->_turnId = _player->getPlayer()->getId();
                        }
                ),
                NULL
        );
        this->runAction(action);
    }
}

Hero *DuelScene2P::getHero(int id) {
    if(_hero1->getPlayer()->getId() == id){
        return _hero2;
    } else if(_hero2->getPlayer()->getId() == id) {
        return _hero1;
    }
    return nullptr;
}