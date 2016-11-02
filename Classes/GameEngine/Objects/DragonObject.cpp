//
// Created by igor on 17.10.16.
//

#include <GameEngine/Global/Variables.h>
#include "DragonObject.h"

USING_NS_CC;

DragonObject::DragonObject() {

    if (this->init()) {
        this->autorelease();
    }
}

DragonObject *DragonObject::create() {
    DragonObject *pSprite = new DragonObject();

    if (pSprite->init()) {
        pSprite->autorelease();

        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return NULL;
}

dragonBones::CCArmatureDisplay *DragonObject::getDisplay() {
    return _armatureDisplay;
}

cocos2d::Node *DragonObject::getNodeByName(std::string name) {
    auto children = this->_armatureDisplay->getChildren();
    for (auto child : children) {
        auto bone = this->_armature->getBoneByDisplay(child);
        if (bone != nullptr && bone->name == name) {
            return child;
        }
    }
    return nullptr;
}

float DragonObject::getGlobalHeight(std::string name) {
    auto node = getNodeByName(name);
    if (node != nullptr) {
        auto bone_transform = _armatureDisplay->getNodeToWorldTransform();
        auto bone_rect = cocos2d::RectApplyTransform(node->getBoundingBox(), bone_transform);
        return bone_rect.getMaxY();
    } else
        return 0.f;
}

void DragonObject::update() {

}

void DragonObject::_updateAnimation() {

}


void DragonObject::addDOChild(cocos2d::Node *target) {
    if (target->getPhysicsBody()) {
        target->removeComponent(target->getPhysicsBody());
    }

    target->retain();
    target->removeFromParentAndCleanup(true);
    //position
    auto globalPoint = Variables::translatePoint(Vec3(0.f, 0.f, 0.f), target, this->getDisplay());
    target->setPosition(globalPoint);
    //scale
    target->setScale(target->getScale() / this->getScale());
    target->setScaleX(
            (getScaleX() > 0) ? target->getScaleX() : ((getScaleX() < 0)
                                                       ? -target->getScaleX() : 0));
    //rotation

//       target->setRotation( * dragonBones::RADIAN_TO_ANGLE);

    this->addChild(target, 1);
    target->release();
}
