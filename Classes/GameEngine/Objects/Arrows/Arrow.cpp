#include <GameEngine/Objects/Environment/Apple.h>
#include <Scenes/PlayLayers/Duel/DuelScene.h>
#include <Scenes/PlayLayers/Apple/AppleBattle.h>
#include <GameEngine/Global/Variables.h>
#include <GameEngine/Objects/Environment/Box.h>
#include <GameEngine/Objects/Environment/HitInfo.h>

USING_NS_CC;

Arrow::Arrow(const std::string &armatureName, float radian, float power, const cocos2d::Vec2 &position,
             int player_id) {
    _damage = 10.f;
    _speedX = std::cos(radian) * power;
    _speedY = -std::sin(radian) * power;

    if (AppleBattle *appleb = dynamic_cast<AppleBattle *>(BattleParent::getInstance())) {
        _x_limit = appleb->visibleSize.width;
    } else
        _x_limit = 0;

    _player_id = player_id;
    _isActive = true;

    _armature = BattleParent::getInstance()->factory.buildArmature(armatureName);
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();

    _head = Vec2(_armature->getBone("head")->global.x, _armature->getBone("head")->global.y);
    _tail = Vec2(_armature->getBone("tail")->global.x, _armature->getBone("tail")->global.y);
    auto dot = Vec2(_armature->getBone("dot")->global.x, _armature->getBone("dot")->global.y);

    auto physicsBody = cocos2d::PhysicsBody::create();
    physicsBody->addShape(PhysicsShapeEdgeSegment::create(_tail, _head));
    physicsBody->setTag(1);
    physicsBody->setMoment(10.f);
    physicsBody->setContactTestBitmask(true);
    this->setPhysicsBody(physicsBody);

    this->setPosition(position);
    this->setRotation(radian * dragonBones::RADIAN_TO_ANGLE);
    this->setScale(BattleParent::getInstance()->getGlobalScale());

    dragonBones::WorldClock::clock.add(_armature);
    this->addChild(_armatureDisplay);
    this->schedule(SEL_SCHEDULE(&Arrow::update), 0.f);
}

Arrow::~Arrow() {
}


void Arrow::update(float dt) {
    if (!_isActive) {
        return;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    const auto &position = this->getPosition();
    const auto &rotation = this->getRotation();

    if (rotation >= 180) {
        _speedY -= BattleParent::G;
    } else {
        _speedY += BattleParent::G;

    }
    this->setPosition(position.x + _speedX, position.y + _speedY);
    this->setRotation(std::atan2(-_speedY, _speedX) * dragonBones::RADIAN_TO_ANGLE);

    auto random = RandomHelper::random_real(0.f, 20.f);

    auto head = Variables::translatePoint(Vec3(_head.x, _head.y, 0.f), _armatureDisplay);

    if (position.y < BattleParent::getInstance()->GROUND + random || (_x_limit && head.x >= _x_limit - 3.f)) {
        _disableArrow();
        return;
    }

    if (position.x < -10.f || position.x >= visibleSize.width + 10.f) {
        this->removeFromParent();
    }
}

void Arrow::_disableArrow() {

    this->stopAllActions();

    if (AppleBattle *appleb = dynamic_cast<AppleBattle *>(BattleParent::getInstance())) {
        appleb->completeShot();
    }

    afterAction();

    this->unscheduleAllCallbacks();

    if (this->getPhysicsBody() != nullptr)
        this->removeComponent(this->getPhysicsBody());

    this->runAction(
            Sequence::create(
                    DelayTime::create(10),
                    CallFunc::create(
                            [&]() {
                                this->removeFromParent();
                            }
                    ),
                    NULL));
}

bool Arrow::processContact(Node *bone) {
    if (bone == nullptr) {
        return false;
    }
    if (AppleParent *appleb = dynamic_cast<AppleParent *>(BattleParent::getInstance())) {
        if (Apple *apple = dynamic_cast<Apple *>(bone)) {
            if(apple->getId() != _player_id) {
                this->addDOChild(apple);

                appleb->setAppleHit();

                auto parent = BattleParent::getInstance();
                parent->addChild(CoinInfo::create(this->getPosition(), parent->getGainedCoinsByActionType(1)));
                parent->addCoins(1);

                return true;
            } else {
                return false;
            }
        }

        if (Body *target = dynamic_cast<Body *>(bone->getParent()->getParent())) {
            if (Hero *hero = dynamic_cast<Hero *>(bone->getParent()->getParent())) {
                if (hero->getPlayer()->getId() == _player_id)
                    return false;
            }

            _disableArrow();

            appleb->setHit();

            return true;
        }
    }


    if (Box *box = dynamic_cast<Box *>(bone)) {

        _disableArrow();

        addToBox(box);

        box->breakBox();

        return true;
    }

    if (Body *target = dynamic_cast<Body *>(bone->getParent()->getParent())) {
        if (Hero *hero = dynamic_cast<Hero *>(bone->getParent()->getParent())) {
            if (hero->getPlayer()->getId() == _player_id)
                return false;
        }

        _disableArrow();

        auto parent = BattleParent::getInstance();

        parent->addChild(DamageInfo::create(this->getPosition(), (int) (Variables::getBoneFactor(bone->getPhysicsBody()->getName()) * _damage)));

        target->dealDamage(_damage, bone);

        addToNode(bone);

        return true;
    }
    return false;
}


void Arrow::_updateAnimation() {

}

void Arrow::update() {

}


void Arrow::addToBox(cocos2d::Node *bone) {
    const auto &position = this->getPosition();
    const auto &rotation = this->getRotation();
    _speedX /= 2;
    _speedY /= 2;
    if (rotation >= 180) {
        _speedY -= BattleParent::G;
    } else {
        _speedY += BattleParent::G;

    }
    this->setPosition(position.x + _speedX, position.y + _speedY);


    this->retain();
    this->removeFromParentAndCleanup(true);
    //position
    auto scenePoint = Variables::translatePoint(Vec3(0.f, 0.f, 0.f), _armatureDisplay);
    auto globalPoint3 = new Vec3(scenePoint.x, scenePoint.y, 0.f);
    bone->getWorldToNodeTransform().transformPoint(globalPoint3);
    auto globalPoint = Vec2(globalPoint3->x, globalPoint3->y);

    this->setPosition(globalPoint);

    //rotation
    auto head = Variables::translatePoint(Vec3(_head.x, _head.y, 0.f), _armatureDisplay,
                                          bone);
    auto tail = Variables::translatePoint(Vec3(_tail.x, _tail.y, 0.f), _armatureDisplay,
                                          bone);
    float x = head.x - tail.x;
    float y = head.y - tail.y;
    auto angle = std::atan2(y, x);

    this->setRotation(-angle * dragonBones::RADIAN_TO_ANGLE);

    bone->addChild(this, 0);
    this->release();
}

void Arrow::addToNode(cocos2d::Node *bone) {
    if (DragonObject *target = dynamic_cast<DragonObject *>(bone->getParent()->getParent())) {

        const auto &position = this->getPosition();
        const auto &rotation = this->getRotation();
        _speedX /= 2;
        _speedY /= 2;
        if (rotation >= 180) {
            _speedY -= BattleParent::G;
        } else {
            _speedY += BattleParent::G;

        }
        this->setPosition(position.x + _speedX, position.y + _speedY);


        this->retain();
        this->removeFromParentAndCleanup(true);
        //position
        auto scenePoint = Variables::translatePoint(Vec3(0.f, 0.f, 0.f), _armatureDisplay);
        scenePoint.x += BattleParent::getInstance()->getPosition().x;
        auto globalPoint3 = new Vec3(scenePoint.x, scenePoint.y, 0.f);
        bone->getWorldToNodeTransform().transformPoint(globalPoint3);
        auto globalPoint = Vec2(globalPoint3->x, globalPoint3->y);

        this->setPosition(globalPoint);
        //scale
        this->setScale(getScale() / target->getScale());
        this->setScaleX(
                (target->getDisplay()->getScaleX() > 0) ? getScaleX() : ((target->getDisplay()->getScaleX() < 0)
                                                                         ? -getScaleX() : 0));

        if (bone->getPhysicsBody()->getName() == "left a" || bone->getPhysicsBody()->getName() == "right a") {
            this->setScaleX(-getScaleX());
        }
        //rotation
        auto head = Variables::translatePoint(Vec3(_head.x, _head.y, 0.f), _armatureDisplay,
                                              bone);
        auto tail = Variables::translatePoint(Vec3(_tail.x, _tail.y, 0.f), _armatureDisplay,
                                              bone);
        float x = head.x - tail.x;
        float y = head.y - tail.y;
        auto angle = std::atan2(y, x);

        this->setRotation(-angle * dragonBones::RADIAN_TO_ANGLE);

        bone->addChild(this, -1);
        this->release();

    }
}

void Arrow::afterAction() {

}
