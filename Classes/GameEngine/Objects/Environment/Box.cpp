#include "Box.h"

USING_NS_CC;

Box *Box::create(BoxType type) {
    Box *ret = new(std::nothrow) Box();
    if (ret && ret->init(type)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;

}

bool Box::init(BoxType type) {
    if (!Node::init()) {
        return false;
    }

    float stoneMass = 20.f;
    float woodMass = 15.f;

    _hp = 3;

    switch (type) {
        case STONE_SMALL_BOX: {
            _box = Sprite::createWithSpriteFrameName("elementStone014.png");
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            physicsBody->setMass(stoneMass);
            this->setPhysicsBody(physicsBody);
            _isBreakble = false;
            _isBroken = false;
            _isStatic = false;
            break;
        }
        case STONE_MEDIUM_BOX: {
            _box = Sprite::createWithSpriteFrameName("elementStone025.png");
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            physicsBody->setMass(stoneMass);
            this->setPhysicsBody(physicsBody);
            _isBreakble = false;
            _isBroken = false;
            _isStatic = false;
            break;
        }
        case STONE_LARGE_BOX: {
            _box = Sprite::createWithSpriteFrameName("elementStone026.png");
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            physicsBody->setMass(stoneMass);
            this->setPhysicsBody(physicsBody);
            _isBreakble = false;
            _isBroken = false;
            _isStatic = false;
            break;
        }
        case WOODEN_BOX: {
            _box = Sprite::createWithSpriteFrameName("elementWood017.png");
            _brokenBox = Sprite::createWithSpriteFrameName("elementWood049.png");
            _brokenBox->retain();
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            physicsBody->setMass(woodMass);
            this->setPhysicsBody(physicsBody);
            _isBreakble = true;
            _isBroken = false;
            _isStatic = false;
            break;
        }
        case WOODEN_TRIANGLE: {
            _box = Sprite::createWithSpriteFrameName("elementWood053.png");
            _brokenBox = Sprite::createWithSpriteFrameName("elementWood042.png");
            _brokenBox->retain();
            Point points[3] = {
                    Point(_box->getBoundingBox().getMinX(), _box->getBoundingBox().getMinY()),
                    Point(_box->getBoundingBox().getMinX(), _box->getBoundingBox().getMaxY()),
                    Point(_box->getBoundingBox().getMaxX(), _box->getBoundingBox().getMinY())
            };
            auto physicsBody = cocos2d::PhysicsBody::createPolygon(points, 3);
            physicsBody->setMass(woodMass);
            this->setPhysicsBody(physicsBody);
            _isBreakble = true;
            _isBroken = false;
            _isStatic = false;
            _hp = 2;
            break;
        }
        case TNT: {
            _box = Sprite::createWithSpriteFrameName("TNT.png");
            _brokenBox = Sprite::createWithSpriteFrameName("TNT.png");
            _brokenBox->retain();
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            physicsBody->setMass(woodMass);
            this->setPhysicsBody(physicsBody);
            _isBreakble = true;
            _isBroken = false;
            _isStatic = false;
            _hp = 1;
            break;
        }
        case GIFT: {
            _box = Sprite::createWithSpriteFrameName("gift.png");
            _brokenBox = Sprite::createWithSpriteFrameName("gift.png");
            _brokenBox->retain();
            auto physicsBody = cocos2d::PhysicsBody::createBox(_box->getBoundingBox().size);
            physicsBody->setMass(woodMass);
            this->setPhysicsBody(physicsBody);
            _isBreakble = true;
            _isBroken = false;
            _isStatic = false;
            _hp = 1;
            break;
        }
        default: {
            return false;
        }
    }



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
