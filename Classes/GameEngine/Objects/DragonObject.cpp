//
// Created by igor on 17.10.16.
//

#include "DragonObject.h"


DragonObject::DragonObject(){

    if (this->init())
    {
        this->autorelease();
    }
}

DragonObject* DragonObject::create()
{
    DragonObject* pSprite = new DragonObject();

    if (pSprite->init())
    {
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

void DragonObject::cleanTransform() {
    this->setScale(1.f);
    this->setPosition(0.f, 0.f);
}

float DragonObject::getNormalScale() {
    return _normalScale;
}

void DragonObject::update() {

}

void DragonObject::_updateAnimation() {

}
