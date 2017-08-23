#include <GameEngine/Objects/Environment/Apple.h>
#include <GameEngine/Objects/WeaponSelector.h>
#include <GameEngine/Objects/Environment/Ground.h>
#include <Localization/LocalizedStrings.h>
#include "DuelScene.h"

USING_NS_CC;

DuelScene *DuelScene::create(Statistics *stats) {
    DuelScene *ret = new(std::nothrow) DuelScene();
    if (ret && ret->init(stats)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    _instance = ret;

    return ret;
}


void DuelScene::initWorld() {

    auto player1 = PlayerDuel::create(_client->getDBPlayer()->getId(),
                                    _client->getDBPlayer()->getName());

    auto player2 =  PlayerDuel::create(-2, LocalizedStrings::getInstance()->getString("BOT"));
    player2->setHAlignment(cocos2d::TextHAlignment::RIGHT);

    _player = new DuelHero(visibleSize.width / 2, DuelScene::GROUND, player1);
    _hero2 = new DuelHero(visibleSize.width * 3 - 150.f, DuelScene::GROUND, player2);
    _hero2->changeFacedir(-1);

    _player1 = _player->getPlayer();
    _player2 = _hero2->getPlayer();

    _brain = new HeroBrainDuel(_hero2, 0.f);

    _createEnvForStickman(_player, _stats->getPlayerEnvType());
    _createEnvForStickman(_hero2, _stats->getTargetEnvType());

    _ui->initDuelBot(visibleSize, _player, _hero2);

    _startGame();

    _setTurnId(_player->getPlayer()->getId());
}

bool DuelScene::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (this->_turnId == _player->getPlayer()->getId()) {
        makeTurn(-1);
        return BattleParent::_touchHandlerBegin(touch, event);
    } else
        return false;
}

bool DuelScene::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return BattleParent::_touchHandlerEnd(touch, event);
}

void DuelScene::makeTurn(int id) {
    if (id == -1 || this->_turnId == 0) {
        _setTurnId(id);
        return;
    }

    if (this->_turnId != id) {
        float delay = 2.f;
        if (id == _player1->getId()) {
            if (this->getPosition().x - _hero2->getPosition().x <= visibleSize.width / 2) {
                delay = 0.5f;
            }
            auto action = Sequence::create(
                    CallFunc::create(
                            [&]() {
                                _setTurnId(_player2->getId());
                            }
                    ),
                    Spawn::createWithTwoActions(
                            MoveTo::create(delay, Vec2(-_hero2->getPosition().x + visibleSize.width / 2, 0.f)),
                            CallFunc::create(
                                    [&, delay]() {
                                        auto vec = Vec2(-_hero2->getPosition().x + visibleSize.width / 2, 0.f);
                                        this->_bg->runAction(MoveTo::create(delay, vec));
                                    }
                            )
                    ),
                    DelayTime::create(1),
                    CallFunc::create(
                            [&]() {
                                _brain->update();
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
                    Spawn::createWithTwoActions(
                            MoveTo::create(delay, Vec2(-_player->getPosition().x + visibleSize.width / 2, 0.f)),
                            CallFunc::create(
                                    [&, delay]() {
                                        auto vec = Vec2(-_player->getPosition().x +
                                                        visibleSize.width / 2, 0.f);
                                        this->_bg->runAction(MoveTo::create(delay, vec));
                                    }
                            )
                    ),
                    CallFunc::create(
                            [&]() {
                                UI::enableArrows(_player, true);
                                _setTurnId(_player1->getId());
                            }
                    ),
                    NULL
            );
            this->runAction(action);
        }
    }
}

bool DuelScene::isGameOver() {
    bool v1 = _player2->getHp() <= 0;
    bool v2 = _player1->getHp() <= 0;
    if(v1){
        _stats->setWin();
    }
    auto isGameOver = v1 || v2;

    return isGameOver;
}

void DuelScene::_startGame() {
    _isStarted = true;

    if(auto waitLayer = this->_ui->getChildByName("PopUp")){
        waitLayer->removeFromParent();
    }

    _ui->startGame();
}

int DuelScene::_getGainedCoinsByActionType(int type) {
    return type;
}

void DuelScene::moveScene(float x) {
    auto current_pos = this->getPosition();
    this->setPosition(Vec2(current_pos.x - x, current_pos.y));
    this->_bg->move(x);
}

int DuelScene::getPlayerId() {
    return _player->getPlayer()->getId();
}


void DuelScene::_setTurnId(int id) {
    this->_turnId = id;
    if (_player1->getId() == id) {
        ((PlayerDuel *)_player1)->markTurn(true);
        ((PlayerDuel *)_player2)->markTurn(false);
    } else if (_player2->getId() == id) {
        ((PlayerDuel *)_player2)->markTurn(true);
        ((PlayerDuel *)_player1)->markTurn(false);
    }
}


