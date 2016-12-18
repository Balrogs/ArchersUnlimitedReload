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
            _box = Sprite::createWithSpriteFrameName("elementStone014.png");
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            this->setPhysicsBody(physicsBody);
            _isBreakble = false;
            _isBroken = false;
            _isStatic = true;
            break;
        }
        case 2: {
            _box = Sprite::createWithSpriteFrameName("elementStone025.png");
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            this->setPhysicsBody(physicsBody);
            _isBreakble = false;
            _isBroken = false;
            _isStatic = true;
            break;
        }
        case 3: {
            _box = Sprite::createWithSpriteFrameName("elementStone026.png");
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            this->setPhysicsBody(physicsBody);
            _isBreakble = false;
            _isBroken = false;
            _isStatic = true;
            break;
        }
        case 4: {
            _box = Sprite::createWithSpriteFrameName("elementWood017.png");
            _brokenBox = Sprite::createWithSpriteFrameName("elementWood049.png");
            _brokenBox->retain();
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            this->setPhysicsBody(physicsBody);
            _isBreakble = true;
            _isBroken = false;
            _isStatic = false;
            break;
        }
        case 5: {
            _box = Sprite::createWithSpriteFrameName("elementWood053.png");
            _brokenBox = Sprite::createWithSpriteFrameName("elementWood042.png");
            _brokenBox->retain();
            Point points[3] = {
                    Point(_box->getBoundingBox().getMinX(), _box->getBoundingBox().getMinY()),
                    Point(_box->getBoundingBox().getMinX(), _box->getBoundingBox().getMaxY()),
                    Point(_box->getBoundingBox().getMaxX(), _box->getBoundingBox().getMinY())
            };
            auto physicsBody = cocos2d::PhysicsBody::createPolygon(points, 3);
            this->setPhysicsBody(physicsBody);
            _isBreakble = true;
            _isBroken = false;
            _isStatic = false;
            break;
        }
        default: {
            return false;
        }
    }
    _hp = 3;
    this->setContentSize(_box->getContentSize());
    auto physicsBody = this->getPhysicsBody();
    physicsBody->setTag(2);
    physicsBody->setContactTestBitmask(true);

    this->addChild(_box, 1);


    return true;
}

void Box::breakBox() {
    if (!_isBreakble) {
        return;
    }

    if (!_isBroken) {
        this->removeChild(_box);
        this->addChild(_brokenBox, 1);
        _brokenBox->release();
        _isBroken = true;
    }

    _hp--;
    if (_hp == 0) {
        this->removeFromParent();
    }
}
