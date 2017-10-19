#include "BombArrow.h"
#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Objects/Hero.h>
#include <Scenes/PlayLayers/BattleParent.h>

BombArrow::BombArrow(const std::string &armatureName, float radian, float power,
                     const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                           player_id) {
    _damage = 50.f;

}

bool BombArrow::processContact(cocos2d::Node *bone) {
    if (Stickman *target = dynamic_cast<Stickman *>(bone->getParent()->getParent())) {
        if (Hero *hero = dynamic_cast<Hero *>(bone->getParent()->getParent())) {
            if (hero->getPlayer()->getId() == _player_id)
                return false;
        }
        _disableArrow();
        return true;
    }
    return false;
}


void BombArrow::afterAction() {
    ParticleExplosion *_emitter = ParticleExplosion::create();

    _emitter->retain();

    auto particle = cocos2d::Sprite::createWithSpriteFrameName("part1.png");

    _emitter->setTotalParticles(1000);
    _emitter->setSpeed(1000.f);
    _emitter->setTexture(particle->getTexture());

    _emitter->setAutoRemoveOnFinish(true);

    _emitter->setPosition(this->getPosition());

    BattleParent::getInstance()->addChild(_emitter);
}

void BombArrow::_disableArrow() {

    afterAction();

    this->retain();

    this->removeFromParent();
}
