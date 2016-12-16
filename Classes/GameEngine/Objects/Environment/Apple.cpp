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

//    auto _emitter = cocos2d::ParticleExplosion::create();
//
//    _emitter->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("fire.png"));
//    _emitter->retain();
//
//    _emitter->setDuration(0.5f);
//    _emitter->setColor(cocos2d::Color3B::YELLOW);
//    _emitter->setLife(0.5f);
//    _emitter->setSpeed(100);
//    _emitter->setEmissionRate(100000);
//
//    _emitter->setAutoRemoveOnFinish(true);
//    _emitter->setPosition(this->getPosition());
//
//    BattleScene::instance->addChild(_emitter, 1);

    BattleScene::instance->addChild(this, 1);
}

Apple::~Apple() {

}


void Apple::hit() {

//    auto _emitter = cocos2d::ParticleSmoke::create();
//    _emitter->retain();
//    _emitter->setLife(0.1f);
//    _emitter->setDuration(0.1f);
//    _emitter->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("fire.png"));
//    auto pos = new cocos2d::Vec3(this->getPosition().x, this->getPosition().y, 0.f);
//    this->getNodeToWorldTransform().transformPoint(pos);
//    _emitter->setPosition(pos->x, pos->y);
//
//    BattleScene::instance->addChild(_emitter, 10);

    this->runAction(
            cocos2d::Sequence::create(
                    cocos2d::DelayTime::create(1.f),
                    cocos2d::CallFunc::create(
                            [&]() {
                                if (AppleBattle *appleb = dynamic_cast<AppleBattle *>(BattleScene::instance)) {
                                    appleb->nextLevelAction();
                                   // BattleScene::instance->removeChild(_emitter);
                                }
                            }
                    ),
                    NULL
            )
    );

}
