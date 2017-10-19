#include "FireArrow.h"
#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Objects/Hero.h>


FireArrow::FireArrow(const std::string &armatureName, float radian, float power,
                     const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                           player_id) {
    _damage = 35.f;

}

bool FireArrow::processContact(cocos2d::Node *bone) {
    return Arrow::processContact(bone);
}

FireArrow::~FireArrow() {

}

void FireArrow::update(float dt) {
    Arrow::update(dt);
}