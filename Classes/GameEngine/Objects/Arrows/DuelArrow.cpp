#include <Scenes/PlayLayers/BattleParent.h>
#include <Scenes/PlayLayers/Duel/DuelScene2P.h>
#include "DuelArrow.h"


void DuelArrow::update(float dt) {

    if (!_isActive) {
        return;
    }
    lifePeriod++;
    const auto &position = this->getPosition();
    const auto &rotation = this->getRotation();

    if (rotation >= 180) {
        _speedY -= BattleParent::G;
    } else {
        _speedY += BattleParent::G;

    }

    this->setPosition(position.x + _speedX, position.y + _speedY);
    this->setRotation(std::atan2(-_speedY, _speedX) * dragonBones::RADIAN_TO_ANGLE);

    if(auto duelScene2P = dynamic_cast<DuelScene2P *>(BattleParent::getInstance())) {
        if(duelScene2P->getPlayerId() == _player_id)
            duelScene2P->moveScene(_speedX);
    } else     if(auto duelScene = dynamic_cast<DuelScene *>(BattleParent::getInstance())) {
        duelScene->moveScene(_speedX);
    }
    auto random = RandomHelper::random_real(0.f, 20.f);

    if (position.y < BattleParent::getInstance()->GROUND + random) {
        _disableArrow();
        if (DuelScene *duel = dynamic_cast<DuelScene *>(BattleParent::getInstance())) {
            duel->makeTurn(_player_id);
        }
    }

}

DuelArrow::DuelArrow(const std::string &armatureName, float radian, float power,
                     const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                           player_id) {

    _damage = 15.f;
    lifePeriod = 0;


    if(auto duelScene = dynamic_cast<DuelScene *>(BattleParent::getInstance())){
        duelScene->makeTurn(-1);
    } else {
        BattleParent::getInstance()->onPopScene();
    }

}

bool DuelArrow::processContact(cocos2d::Node *bone) {
    if (Arrow::processContact(bone))
        if (DuelScene *duel = dynamic_cast<DuelScene *>(BattleParent::getInstance())) {
            duel->makeTurn(_player_id);
            return true;
        }
    return false;
}

DuelArrow::~DuelArrow() {

}
