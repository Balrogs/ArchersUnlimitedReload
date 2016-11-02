//
// Created by igor on 21.09.16.
//

#include <Scenes/PlayLayers/Battle.h>
#include <Scenes/PlayLayers/AppleBattle.h>
#include "Apple.h"

Apple::Apple(float x_pos, float y_pos) {

    auto texture = cocos2d::Sprite::createWithSpriteFrameName("apple.png");

    this->setPosition(x_pos, y_pos);
    this->setScale(BattleScene::instance->getGlobalScale());

    cocos2d::Size size = texture->getContentSize();

    auto physicsBody = cocos2d::PhysicsBody::createCircle(size.width / 2,
                                                          cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);
    physicsBody->setTag(2);
    physicsBody->setContactTestBitmask(true);
    physicsBody->setVelocityLimit(0);
    this->setPhysicsBody(physicsBody);

    this->addChild(texture, 0);

    //effect
    auto particle = cocos2d::Sprite::createWithSpriteFrameName("part1.png");

    auto _emitter = cocos2d::ParticleFlower::create();
    _emitter->retain();

    _emitter->setDuration(0.5f);

    _emitter->setTexture(particle->getTexture());
    _emitter->setLife(1);
    _emitter->setSpeed(100);
    _emitter->setEmissionRate(100000);

    _emitter->setAutoRemoveOnFinish(true);
    _emitter->setPosition(this->getPosition());

    BattleScene::instance->addChild(_emitter);

    BattleScene::instance->addChild(this, 1);
}

Apple::~Apple() {

}


void Apple::hit() {
    this->runAction(
            cocos2d::Sequence::create(
                    cocos2d::DelayTime::create(1.f),
                    cocos2d::CallFunc::create(
                            [&]() {
                                if (AppleBattle *appleb = dynamic_cast<AppleBattle *>(BattleScene::instance)) {
                                    appleb->nextLevelAction();
                                }
                            }
                    ),
                    NULL
            )
    );

}
