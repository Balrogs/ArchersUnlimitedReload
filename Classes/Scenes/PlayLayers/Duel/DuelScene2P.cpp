#include <GameEngine/Objects/Environment/Ground.h>
#include <Localization/LocalizedStrings.h>
#include "DuelScene2P.h"

USING_NS_CC;

DuelScene2P *DuelScene2P::create(Statistics *stats) {
    DuelScene2P *ret = new(std::nothrow) DuelScene2P();
    if (ret && ret->init(stats)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    _instance = ret;

    return ret;
}

void DuelScene2P::initWorld() {

    auto player1 = PlayerDuel::create(1, StringUtils::format("%s 1", LocalizedStrings::getInstance()->getString("PLAYER")).c_str());

    auto player2 = PlayerDuel::create(2, StringUtils::format("%s 2", LocalizedStrings::getInstance()->getString("PLAYER")).c_str());
    player2->setHAlignment(cocos2d::TextHAlignment::RIGHT);

    _hero1 = new DuelHero(visibleSize.width / 2, DuelScene::GROUND, player1);
    _hero2 = new DuelHero(visibleSize.width * 3 - 150.f, DuelScene::GROUND, player2);

    _hero2->changeFacedir(-1);

    _player1 = _hero1->getPlayer();
    _player2 = _hero2->getPlayer();

    _createEnvForStickman(_hero1, _stats->getPlayerEnvType());
    _createEnvForStickman(_hero2, _stats->getTargetEnvType());

    _ui->initDuel(visibleSize, _hero1, _hero2);
    _player = _hero1;

    _startGame();

    _setTurnId(_player->getPlayer()->getId());
}

bool DuelScene2P::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (this->_turnId == _player->getPlayer()->getId()) {
        return BattleParent::_touchHandlerBegin(touch, event);
    } else
        return false;
}

void DuelScene2P::makeTurn(int id) {
    if (id == -1 || this->_turnId == 0) {
        _setTurnId(id);
        return;
    }

    if (this->_turnId != id) {
        if(id == _player1->getId()){
            BattleHistory::getInstance()->nextRound();
        }

        float delay = 2.f;
        if (this->getPosition().x >= visibleSize.width / 2) {
            delay = 0.5f;
        }
        _player = getHero(id);
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
                            _setTurnId(_player->getPlayer()->getId());
                        }
                ),
                NULL
        );
        this->runAction(action);
    }
}
