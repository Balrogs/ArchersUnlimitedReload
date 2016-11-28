
#include <GameEngine/Global/Misc/SocketClient.h>
#include <GameEngine/Objects/Environment/Ground.h>
#include "DuelSceneMultiplayer.h"

USING_NS_CC;

void DuelSceneMultiplayer::initWorld() {
    _isStarted = false;
    _isGamePaused = false;

    _client = SocketClient::getInstance();

    Ground *ground = new Ground(GROUND, visibleSize.width * 4);
    this->addChild(ground);

}

bool DuelSceneMultiplayer::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_isStarted && !_isGamePaused && this->_turnId == _client->getDBPlayer()->getId()) {
        return BattleScene::_touchHandlerBegin(touch, event);
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

void DuelSceneMultiplayer::onEnter() {
    Node::onEnter();
    _client = SocketClient::getInstance();
}

bool DuelSceneMultiplayer::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();
    float x = start.x - curr.x;
    float y = start.y - curr.y;
    auto power = std::sqrt(x * x + y * y);
    power = power / 10;
    power = (power > MAX_ARROW_POWER) ? MAX_ARROW_POWER : power;
    power = (power < MIN_ARROW_POWER) ? MIN_ARROW_POWER : power;
    auto angle = std::atan2(y, x);
    _hero1->attack(angle, power);
    _client->action(angle, power, 1);
    return true;
}

void DuelSceneMultiplayer::receiveAction(float angle, float power) {
    _player->attack(angle, power);
    _client->action(angle, power, 1);
}

void DuelSceneMultiplayer::setPlayer(int id) {

    switch (id) {
        case 1:{
            _hero1 = new DuelHero(visibleSize.width / 2, DuelScene::GROUND, _client->getDBPlayer()->getId(), _client->getDBPlayer()->getName().c_str());

            _hero2 = new DuelHero(visibleSize.width * 3 - 150.f, DuelScene::GROUND, _client->getDBPlayer()->getName().c_str());

            _hero2->changeFacedir(-1);
            _player1 = _hero1->getPlayer();
            _player2 = _hero2->getPlayer();
            _player2->setHAlignment(cocos2d::TextHAlignment::RIGHT);

            ui->initDuel(visibleSize, _hero1, _hero2);
            _player = _hero1;
        }

            break;
        case 2: {
            _hero1 = new DuelHero(visibleSize.width / 2, DuelScene::GROUND,  _client->getDBPlayer()->getName().c_str());

            _hero2 = new DuelHero(visibleSize.width * 3 - 150.f, DuelScene::GROUND, _client->getDBPlayer()->getId(), _client->getDBPlayer()->getName().c_str());

            _hero2->changeFacedir(-1);
            _player1 = _hero1->getPlayer();
            _player2 = _hero2->getPlayer();
            _player2->setHAlignment(cocos2d::TextHAlignment::RIGHT);

            ui->initDuel(visibleSize, _hero1, _hero2);
            _player = _hero2;
        }
            break;
        default:
            break;
    }

    auto action = Sequence::create(
            MoveTo::create(0.f, Vec2(-_player->getPosition().x + visibleSize.width / 2, 0.f)),
            NULL
    );
    this->runAction(action);
    this->_turnId = _player->getPlayer()->getId();
}

void DuelSceneMultiplayer::startGame() {
    _isStarted = true;
}

void DuelSceneMultiplayer::pauseGame() {
    _isGamePaused = true;
}

void DuelSceneMultiplayer::resumeGame() {
    _isGamePaused = false;
}
