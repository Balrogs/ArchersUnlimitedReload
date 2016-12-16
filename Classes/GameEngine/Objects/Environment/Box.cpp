//
// Created by igor on 16.12.16.
//

#include "Box.h"

USING_NS_CC;

Box *Box::create(int type) {
    Box *ret = new(std::nothrow) Box();
    if (ret && ret->init(type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;

}

bool Box::init(int type) {
    if (!Node::init()) {
        return false;
    }
    
    int count = 4;
    switch (type) {
        case 1: {
            _box = Sprite::createWithSpriteFrameName("");
            break;
        }
        case 2: {
            _box = Sprite::createWithSpriteFrameName("");
            break;
        }
        case 3: {
            _box = Sprite::createWithSpriteFrameName("");
            break;
        }
        case 4: {
            _box = Sprite::createWithSpriteFrameName("");
            break;
        }
        case 5: {
            _box = Sprite::createWithSpriteFrameName("");
            break;
        }
        default: {
            return false;
        }
    }

    this->setContentSize(_box->getContentSize());
    auto physicsBody = cocos2d::PhysicsBody::createPolygon(points, count);
    physicsBody->setTag(2);
    physicsBody->setContactTestBitmask(true);
    this->setPhysicsBody(physicsBody);

    this->addChild(_box, 1);

    return true;
}

void Box::breakBox() {
    if(_isBreakble){
        return;
    }
    if(!_isBroken){
        this->removeChild(_box);
        this->addChild(_brokenBox, 1);
    }
}
