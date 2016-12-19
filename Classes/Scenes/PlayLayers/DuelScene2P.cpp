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
    return ret;
}

void DuelScene2P::initWorld() {

    _hero1 = new DuelHero(visibleSize.width / 2, DuelScene::GROUND,
                          StringUtils::format("%s 1", LocalizedStrings::getInstance()->getString("PLAYER")).c_str());

    _hero2 = new DuelHero(visibleSize.width * 3 - 150.f, DuelScene::GROUND,
                          StringUtils::format("%s 2", LocalizedStrings::getInstance()->getString("PLAYER")).c_str());

    _hero2->changeFacedir(-1);
    _player1 = _hero1->getPlayer();
    _player2 = _hero2->getPlayer();
    _player2->setHAlignment(cocos2d::TextHAlignment::RIGHT);

    _createEnvForStickman(_hero1, _stats->getPlayerEnvType());
    _createEnvForStickman(_hero2, _stats->getTargetEnvType());

    _ui->initDuel(visibleSize, _hero1, _hero2);
    _player = _hero1;

    _startGame();
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
                            this->_turnId = _player->getPlayer()->getId();
                        }
                ),
                NULL
        );
        this->runAction(action);
    }
}

Hero *DuelScene2P::getHero(int id) {
    if (_hero1->getPlayer()->getId() == id) {
        return _hero2;
    } else if (_hero2->getPlayer()->getId() == id) {
        return _hero1;
    }
    return nullptr;
}
