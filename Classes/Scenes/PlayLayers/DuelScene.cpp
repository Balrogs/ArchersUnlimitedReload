#include <GameEngine/Objects/Environment/Apple.h>
#include <GameEngine/Global/WeaponSelector.h>
#include <GameEngine/Objects/Environment/Ground.h>
#include "DuelScene.h"

USING_NS_CC;

void DuelScene::initWorld() {

    Ground *ground = new Ground(GROUND, visibleSize.width * 4);
    this->addChild(ground);

    _player = new DuelHero(visibleSize.width / 2, DuelScene::GROUND, "HEROOOOOOOOOO");

    auto target = new DuelHero(visibleSize.width * 3 - 150.f, DuelScene::GROUND, "BOT");

    _player1 = _player->getPlayer();
    _player2 = target->getPlayer();
    _player2->setHAlignment(cocos2d::TextHAlignment::RIGHT);
    _targets.push_back(target);
    _brains.push_back(new HeroBrainDuel(target, 0.f));

    ui->initDuel(visibleSize, _player, target);

    makeTurn(_player2->getId());
}

bool DuelScene::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (this->_turnId == _player2->getId()) {
        makeTurn(-1);
        return BattleScene::_touchHandlerBegin(touch, event);
    } else
        return false;
}

bool DuelScene::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return BattleScene::_touchHandlerEnd(touch, event);
}

void DuelScene::makeTurn(int id) {
    if (id == -1 || this->_turnId == 0) {
        this->_turnId = id;
        return;
    }

    if (this->_turnId != id) {
        float delay = 2.f;
        if (id == _player1->getId()) {
            if (this->getPosition().x - _targets.at(0)->getPosition().x <= visibleSize.width / 2) {
                delay = 0.5f;
            }
            auto action = Sequence::create(
                    CallFunc::create(
                            [&]() {
                                this->_turnId = _player1->getId();
                            }
                    ),
                    MoveTo::create(delay,
                                   Vec2(-_targets.at(0)->getPosition().x + visibleSize.width / 2, 0.f)),
                    DelayTime::create(1),
                    CallFunc::create(
                            [&]() {
                                _brains[0]->update();
                            }
                    ),
                    NULL
            );
            this->runAction(action);
        } else if (id == _player2->getId()) {

            if (this->getPosition().x >= visibleSize.width / 2) {
                delay = 0.5f;
            }
            auto action = Sequence::create(
                    MoveTo::create(delay, Vec2::ZERO),
                    CallFunc::create(
                            [&]() {
                                UI::enableArrows(_player, true);
                                this->_turnId = _player2->getId();
                            }
                    ),
                    NULL
            );
            this->runAction(action);
        }
    }
}

bool DuelScene::isGameOver() {
    return (_player1->getShotsCount() == _player2->getShotsCount() && _player1->getShotsCount() >= 15) ||
           _player1->getHp() <= 0 || _player2->getHp() <= 0;
}

void DuelScene::_keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    BattleScene::_keyBoardPressedHandler(keyCode, event);

}

void DuelScene::_keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    BattleScene::_keyBoardReleasedHandler(keyCode, event);
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _player->move(-1);
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _player->move(1);
            break;
    }
}