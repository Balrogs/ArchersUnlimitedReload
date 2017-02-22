#include <GameEngine/Global/Variables.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include "MultiplayerBattle.h"
#include <math.h>
#include <Localization/LocalizedStrings.h>

void MultiplayerBattle::createPlayers(Player *player1, Player *player2) {
    _player1 = player1;
    _player2 = player2;

    waitForPlayer();
}


void MultiplayerBattle::receiveAction(float angle, float power, int id, int x, int y) {
    if (id != _client->getDBPlayer()->getId()) {
         CCLOG("RECEIVED : angle : %f , power : %f",angle, power);
        _player->attack(angle, power);
    }
}

void MultiplayerBattle::receiveMove(int dir, int id) {
    if (id != _client->getDBPlayer()->getId()) {
        _player->move(dir);
    }
}

void MultiplayerBattle::receiveAim(float angle, float power, int id) {
    if (id != _client->getDBPlayer()->getId()) {
//        CCLOG("RECEIVED : angle : %f , power : %f",angle, power);
        _player->setAim(angle, power);
    }
}

void MultiplayerBattle::setPlayer(int id) {
    _playerId = _player1->getId();
    switch (id) {
        case 1: {
            _hero1->setPlayer(_player1);
            _hero2->setPlayer(_player2);
        }

            break;
        case 2: {
            _hero1->setPlayer(_player2);
            _hero2->setPlayer(_player1);
        }
            break;
        default:
            break;
    }

    _player = _hero1;
    _hero2->changeFacedir(-1);
    _player1 = _hero1->getPlayer();
    _player2 = _hero2->getPlayer();
    _player2->setHAlignment(cocos2d::TextHAlignment::RIGHT);

    _createEnvForStickman(_hero1, _stats->getPlayerEnvType());
    _createEnvForStickman(_hero2, _stats->getTargetEnvType());

    this->_turnId = _hero1->getPlayer()->getId();
}

void MultiplayerBattle::startGame() {
    _startGame();

    auto action = Sequence::create(
            MoveTo::create(0.f, Vec2(-_player->getPosition().x + visibleSize.width / 2, 0.f)),
            NULL
    );
    this->runAction(action);

}

void MultiplayerBattle::pauseGame() {
    BattleParent::pauseBattle();
    // TO DO send game paused to server
}

void MultiplayerBattle::resumeGame() {
    BattleParent::unPause();
}

void MultiplayerBattle::abort() {
    auto popUp = PopUp::create(LocalizedStrings::getInstance()->getString("GAME OVER"),
                               cocos2d::Label::createWithTTF(LocalizedStrings::getInstance()->getString("OPPONENT HAS LEFT THE GAME"), Variables::FONT_NAME, Variables::FONT_SIZE),
                               false);
    popUp->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->_ui->addChild(popUp, 0, "PopUp");
}

void MultiplayerBattle::onEnter() {
    Node::onEnter();
    _client = SocketClient::getInstance();
}

bool MultiplayerBattle::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_touch == touch->getID()) {
        _touch = -1;
    } else {
        return false;
    }

    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();
    float x = start.x - curr.x;
    float y = start.y - curr.y;
    auto power = std::sqrt(x * x + y * y);
    power = power / 10;
    power = (power > MAX_ARROW_POWER) ? MAX_ARROW_POWER : power;
    power = (power < MIN_ARROW_POWER) ? MIN_ARROW_POWER : power;
    auto angle = - std::atan2(y, x);

    // round values
    float k = std::pow(10, 4);
    power =  roundf(power * k) / k;
    angle =  roundf(angle * k) / k;

    CCLOG("angle %f power %f", angle, power);

    _player->attack(angle, power);

    return true;
}

bool MultiplayerBattle::_touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_touch != touch->getID()) {
        return false;
    }

    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();
    float x = start.x - curr.x;
    float y = start.y - curr.y;
    auto angle = std::atan2(y, x);
    auto power = std::sqrt(x * x + y * y);
    power = power / 10;
    power = (power > MAX_ARROW_POWER) ? MAX_ARROW_POWER : power;
    power = (power < MIN_ARROW_POWER) ? MIN_ARROW_POWER : power;

    float k = std::pow(10, 4);
    power =  roundf(power * k) / k;
    angle =  roundf(angle * k) / k;

    _player->setAim(angle, power);
    if(_player->checkAimDiff(angle, power))
    _client->aim(angle, power);
    return true;
}

void MultiplayerBattle::onPopScene() {
    _client->gameOver(-1, -1);
    BattleParent::onPopScene();
}

int MultiplayerBattle::getPlayerId() {
    return _playerId;
}

void MultiplayerBattle::movePlayer(int dir) {
    _client->move(dir);
}

void MultiplayerBattle::waitForPlayer() {

    if(auto child = this->getChildByName("PopUp")) {
        child->removeFromParent();
    }
    auto popUp = WaitingPopUp::create();
    popUp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->_ui->addChild(popUp, 10, "PopUp");
}
