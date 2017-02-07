#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include "MultiplayerBattle.h"

void MultiplayerBattle::createPlayers(Player *player1, Player *player2) {
    _player1 = player1;
    _player2 = player2;
}


void MultiplayerBattle::receiveAction(float angle, float power, int id) {
    if (id != _client->getDBPlayer()->getId()) {
        _player->attack(angle, power);
    }
}

void MultiplayerBattle::setPlayer(int id) {
    switch (id) {
        case 1: {
            _hero1 = new DuelHero(visibleSize.width / 2, BattleParent::GROUND, _player1);
            _hero2 = new DuelHero(visibleSize.width * 3 - 150.f, BattleParent::GROUND, _player2);
            _player = _hero1;
        }

            break;
        case 2: {
            _hero1 = new DuelHero(visibleSize.width / 2, BattleParent::GROUND, _player2);
            _hero2 = new DuelHero(visibleSize.width * 3 - 150.f, BattleParent::GROUND, _player1);
            _player = _hero2;
        }
            break;
        default:
            break;
    }

    _hero2->changeFacedir(-1);
    _player1 = _hero1->getPlayer();
    _player2 = _hero2->getPlayer();
    _player2->setHAlignment(cocos2d::TextHAlignment::RIGHT);

    _createEnvForStickman(_hero1, _stats->getPlayerEnvType());
    _createEnvForStickman(_hero2, _stats->getTargetEnvType());

    auto action = Sequence::create(
            MoveTo::create(0.f, Vec2(-_player->getPosition().x + visibleSize.width / 2, 0.f)),
            NULL
    );
    this->runAction(action);

    this->_turnId = _hero1->getPlayer()->getId();
}

void MultiplayerBattle::startGame() {
    _isStarted = true;
    _startGame();
}

void MultiplayerBattle::pauseGame() {
    BattleParent::pauseBattle();
    // TO DO send game paused to server
}

void MultiplayerBattle::resumeGame() {
    BattleParent::unPause();
}

void MultiplayerBattle::abort() {
    this->addChild(PopUp::create("GAME OVER",
                                 cocos2d::Label::createWithTTF("OPPONENT HAS LEFT THE GAME", Variables::FONT_NAME,
                                                               20.f)), 10, "PopUp");
}

void MultiplayerBattle::onEnter() {
    Node::onEnter();
    _client = SocketClient::getInstance();
}

bool MultiplayerBattle::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
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

void MultiplayerBattle::onPopScene() {
    _client->gameOver(-1, 1);
    BattleParent::onPopScene();
}