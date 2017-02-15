//
// Created by igor on 17.10.16.
//

#include <GameEngine/Global/Variables.h>
#include <dragonBones/cocos2dx/CCFactory.h>
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

HeroPreview::HeroPreview() {
    dragonBones::CCFactory factory;
    const auto dragonBonesData = factory.loadDragonBonesData("ArcUnlimArmature.json");
    factory.loadTextureAtlasData("texture.json");

    _armature = factory.buildArmature("Stickman");
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();

    //TODO add some animations
    _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);

    _armature->removeSlot(_armature->getSlot("Apple"));

    auto _shoulders = _armature->getSlot("Hands")->getChildArmature();
    auto _shouldersDisplay = (dragonBones::CCArmatureDisplay *) _shoulders->getDisplay();


    const auto firePointBone = _armature->getBone("shoulders");
    cocos2d::Vec2 globalPoint;
    globalPoint.set(firePointBone->global.x, -firePointBone->global.y);
    _shouldersDisplay->setPosition(globalPoint);
    _shouldersDisplay->setAnchorPoint(globalPoint);

    _armature->removeBone(_armature->getBone("shoulders"));

    _bowArmature = _shoulders->getSlot("Bow")->getChildArmature();
    _bowArmatureDisplay = (dragonBones::CCArmatureDisplay *) _bowArmature->getDisplay();
    _arrowArmature = _shoulders->getSlot("Arrow")->getChildArmature();
    _arrowDisplay = (dragonBones::CCArmatureDisplay *) _arrowArmature->getDisplay();

    _string = cocos2d::Node::create();

    _updateString();

    _bowArmatureDisplay->addChild(_string);

    dragonBones::WorldClock::clock.add(_armature);
    this->addChild(_armatureDisplay);
}

void HeroPreview::changeHat() {

}

void HeroPreview::changeBow() {

}

void HeroPreview::changeArrow() {

}

void HeroPreview::_updateString() {

    _string->removeAllChildren();

    auto top = _bowArmature->getBone("top");
    auto bottom = _bowArmature->getBone("bottom");


    auto line = cocos2d::DrawNode::create();
    line->drawLine(cocos2d::Vec2(top->global.x, -top->global.y), cocos2d::Vec2(bottom->global.x, -bottom->global.y),
                   cocos2d::Color4F::BLACK);

}
