#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Objects/Hero.h>
#include "FrozenArrow.h"


FrozenArrow::FrozenArrow(const std::string &armatureName, float radian, float power,
                         const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                               player_id) {
    _damage = 20.f;

}

bool FrozenArrow::processContact(cocos2d::Node *bone) {
    return Arrow::processContact(bone);
}

FrozenArrow::~FrozenArrow() {

}

void FrozenArrow::update(float dt) {
    Arrow::update(dt);
}