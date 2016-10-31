//
// Created by igor on 21.09.16.
//

#include <Scenes/PlayLayers/Battle.h>
#include "Apple.h"

Apple::Apple(float x_pos, float y_pos) : DragonObject() {

    _x_pos = x_pos;
    _y_pos = y_pos;
    _normalScale = 0.3f;
    _armature = BattleScene::instance->factory.buildArmature("Apple");
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();

    _y_pos += this->getGlobalHeight("root") / 4;

    this->setPosition(_x_pos, _y_pos);
    this->setScale(_normalScale * BattleScene::instance->getGlobalScale());
    _updateAnimation();

    cocos2d::Size size = _armatureDisplay->getChildren().at(0)->getContentSize();
    auto physicsBody = cocos2d::PhysicsBody::createCircle(size.width / 2,
                                                          cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);
    physicsBody->setTag(2);
    physicsBody->setContactTestBitmask(true);
    this->setPhysicsBody(physicsBody);

    dragonBones::WorldClock::clock.add(_armature);
    this->addChild(_armatureDisplay, 0);
    BattleScene::instance->addChild(this, 1);
}

Apple::~Apple() {

}

void Apple::update() {

}

void Apple::_updateAnimation() {

}
