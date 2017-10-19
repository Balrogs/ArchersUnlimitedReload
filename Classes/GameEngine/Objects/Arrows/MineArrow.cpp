#include "MineArrow.h"


MineArrow::MineArrow(const std::string &armatureName, float radian, float power,
                     const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                           player_id) {
    _damage = 50.f;

}

bool MineArrow::processContact(cocos2d::Node *bone) {
    return Arrow::processContact(bone);
}

MineArrow::~MineArrow() {

}

void MineArrow::update(float dt) {

    Arrow::update(dt);
}