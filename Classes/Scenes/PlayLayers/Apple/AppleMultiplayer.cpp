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

void AppleMultiplayer::initWorld() {
    _isStarted = false;

    _client = SocketClient::getInstance();

}

bool AppleMultiplayer::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_isStarted && this->_turnId == _client->getDBPlayer()->getId()) {
        return BattleParent::_touchHandlerBegin(touch, event);
    } else
        return false;
}

void AppleMultiplayer::makeTurn(int id) {
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
                        )),
                CallFunc::create(
                        [&]() {
                            if (_player->getPlayer()->getId() == _playerId)
                                UI::enableArrows(_player, true);
                            setTurnId(_player->getPlayer()->getId());
                        }
                ),
                NULL
        );
        this->runAction(action);
    }
}

void AppleMultiplayer::setTurnId(int id) {
    this->_turnId = id;
}

void AppleMultiplayer::setPlayer(int id) {
    MultiplayerBattle::setPlayer(id);
    _ui->initDuel(visibleSize, _hero1, _hero2);
}

void AppleMultiplayer::addApple() {

}

void AppleMultiplayer::initObjects() {

}

bool AppleMultiplayer::isGameOver() {
    return DuelScene::isGameOver();
}

void AppleMultiplayer::_nextLevelAction() {

}
