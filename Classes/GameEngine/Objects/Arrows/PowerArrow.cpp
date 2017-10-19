#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Objects/Hero.h>
#include "PowerArrow.h"


PowerArrow::PowerArrow(const std::string &armatureName, float radian, float power,
                       const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                             player_id) {
    _damage = 40.f;
}


bool PowerArrow::processContact(Node *bone) {
    if (bone == nullptr) {
        return false;
    }
    if (Stickman *target = dynamic_cast<Stickman *>(bone->getParent()->getParent())) {
        if (Hero *hero = dynamic_cast<Hero *>(bone->getParent()->getParent())) {
            if (hero->getPlayer()->getId() == _player_id)
                return false;
        }

        _disableArrow();

        //  target->dealDamage(_damage);

        target->hit(cocos2d::Vec2(_speedX, _speedY));

        addToNode(bone);

        return true;
    }
    return false;
}

PowerArrow::~PowerArrow() {

}