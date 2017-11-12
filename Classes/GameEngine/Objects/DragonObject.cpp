//
// Created by igor on 17.10.16.
//

#include <GameEngine/Global/Variables.h>
#include <dragonBones/cocos2dx/CCFactory.h>
#include <dragonBones/cocos2dx/CCSlot.h>
#include <Scenes/MenuLayers/Main/EquipmentScene.h>
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

    _playerView = PlayerView::readPlayerView();

    _armature = EquipmentScene::getInstance()->factory.buildArmature("Stickman");
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();

    _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);

     _shoulders = _armature->getSlot("Hands")->getChildArmature();
     _shouldersDisplay = (dragonBones::CCArmatureDisplay *) _shoulders->getDisplay();

    const auto firePointBone = _armature->getBone("shoulders");
    cocos2d::Vec2 globalPoint;
    globalPoint.set(firePointBone->global.x, -firePointBone->global.y);
    _shouldersDisplay->setPosition(globalPoint);
    _shouldersDisplay->setAnchorPoint(globalPoint);

    _armature->removeBone(_armature->getBone("shoulders"));

    _bow = EquipmentScene::getInstance()->factory.buildArmature(_playerView->getBow()->Path());

    _bowArmatureDisplay = (dragonBones::CCArmatureDisplay *) _bow->getDisplay();
    _string = cocos2d::Node::create();
    _bowArmatureDisplay->addChild(_string);
    _setPlayerView();

    _updateString();

    dragonBones::WorldClock::clock.add(_armature);
    this->addChild(_armatureDisplay);


}

void HeroPreview::changeHat(int id) {

    CCLOG("HAT ID %d",id);
    if(id == -1){
        _changeHat("Hat");
    } else {
        _playerView->setHat(id);
        _changeHat();

    }
}

void HeroPreview::changeBow(int id) {
    if(id == -1){
        _changeBow("Hat");
    } else {
        _playerView->setBow(id);
        _changeBow();

    }
}

void HeroPreview::changeArrow(int id) {
    if(id == -1){
        _changeArrow("Hat");
    } else {

        _playerView->setArrow(id);
        _changeArrow();

    }
}

void HeroPreview::_updateString() {

    _string->removeAllChildren();

    auto top = _bow->getBone("top");
    auto bottom = _bow->getBone("bottom");

    if(top == nullptr || bottom == nullptr){
        return;
    }

    auto line = cocos2d::DrawNode::create();
    line->drawLine(cocos2d::Vec2(top->global.x, -top->global.y), cocos2d::Vec2(bottom->global.x, -bottom->global.y),
                   Color4F::GRAY);
    _string->addChild(line);
}

void HeroPreview::_setPlayerView() {
    _changeArrow();
    _changeBow();
    _changeHat();
}

void HeroPreview::_changeArrow(std::string path) {
    _arrow = EquipmentScene::getInstance()->factory.buildArmature(path);
    _shoulders->getSlot("Arrow")->setChildArmature(_arrow);
}

void HeroPreview::_changeHat(std::string path) {
    _hat = EquipmentScene::getInstance()->factory.buildArmature(path);
    _armature->getSlot("Hat")->setChildArmature(_hat);
}

void HeroPreview::_changeBow(std::string path) {

    _bow = EquipmentScene::getInstance()->factory.buildArmature(path);

    _shoulders->getSlot("Bow")->setChildArmature(_bow);

    _bowArmatureDisplay = (dragonBones::CCArmatureDisplay *) _bow->getDisplay();
    _bowArmatureDisplay->addChild(_string);

    _updateString();
}

float HeroPreview::getHatHeight() {
    auto node = getNodeByName("head");
    if (node != nullptr) {
        auto bone_rect = node->getBoundingBox();
        return bone_rect.getMinY();
    }
    return 0;
}

Vec2 HeroPreview::getSlotPosition(std::string name) {
    auto node = _armature->getBone(name);
    Vec3 point;
    if (node == nullptr) {
        node = _shoulders->getBone(name);
        auto transform = node->global;
        auto p = new Vec3(transform.x, -transform.y, 0);
        _shouldersDisplay->getNodeToParentTransform().transformPoint(p);
        point = Vec3(p->x, p->y, 0);;
    } else {
        auto transform = node->global;
        point = Vec3(transform.x, -transform.y, 0);
    }

    this->getWorldToNodeTransform().translate(point);
    return Vec2(point.x, point.y);
}

PlayerView *HeroPreview::getPlayerView() {
    return _playerView;
}

void HeroPreview::_changeArrow() {
    _changeArrow(_playerView->getArrow()->Path());
}

void HeroPreview::_changeHat() {
    _changeHat(_playerView->getHat()->Path());
}

void HeroPreview::_changeBow() {
    _changeBow(_playerView->getBow()->Path());
}

dragonBones::Armature *HeroPreview::getShoulders() {
    return _shoulders;
}