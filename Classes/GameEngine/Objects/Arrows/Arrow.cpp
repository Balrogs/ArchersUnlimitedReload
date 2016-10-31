#include <GameEngine/Objects/Environment/Apple.h>
#include <Scenes/PlayLayers/DuelScene.h>
#include <Scenes/PlayLayers/AppleBattle.h>
#include <GameEngine/Global/Variables.h>

USING_NS_CC;


Arrow::Arrow(const std::string &armatureName, float radian, float power, const cocos2d::Vec2 &position,
             int player_id) {

    _normalScale = 0.4f;

    _speedX = std::cos(radian) * power;
    _speedY = -std::sin(radian) * power;

    if (AppleBattle *appleb = dynamic_cast<AppleBattle *>(BattleScene::instance)) {
        _x_limit = appleb->visibleSize.width;
    } else
        _x_limit = 0;

    _player_id = player_id;
    _isActive = true;

    _armature = BattleScene::instance->factory.buildArmature(armatureName);
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();

    _head = Vec2(_armature->getBone("head")->global.x, _armature->getBone("head")->global.y);
    _tail = Vec2(_armature->getBone("tail")->global.x, _armature->getBone("tail")->global.y);

    auto physicsBody = cocos2d::PhysicsBody::create();
    physicsBody->addShape(PhysicsShapeEdgeSegment::create(_tail, _head));
    physicsBody->setTag(1);
    physicsBody->setMoment(10.f);
    physicsBody->setContactTestBitmask(true);
    this->setPhysicsBody(physicsBody);

    this->setPosition(position);
    this->setRotation(radian * dragonBones::RADIAN_TO_ANGLE);
    this->setScale(_normalScale * BattleScene::instance->getGlobalScale());

    dragonBones::WorldClock::clock.add(_armature);
    this->addChild(_armatureDisplay);
    this->schedule(SEL_SCHEDULE(&Arrow::update), 0.1f);
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
        _speedY -= BattleScene::G;
    } else {
        _speedY += BattleScene::G;

    }
    this->setPosition(position.x + _speedX, position.y + _speedY);
    this->setRotation(std::atan2(-_speedY, _speedX) * dragonBones::RADIAN_TO_ANGLE);

    auto random = RandomHelper::random_real(0.f, 20.f);

    auto head = Variables::translatePoint(Vec3(_head.x, _head.y, 0.f), _armatureDisplay);

    if (position.y < BattleScene::GROUND + random || (_x_limit && head.x >= _x_limit - 3.f)) {
        _disableArrow();
        return;
    }

    if (position.x < -10.f || position.x >= visibleSize.width + 10.f) {
        this->removeFromParent();
    }
}

void Arrow::_disableArrow() {

    afterAction();

    this->unscheduleAllCallbacks();

    this->removeComponent(this->getPhysicsBody());
}

bool Arrow::processContact(Node *bone) {
    if (bone == nullptr) {
        return false;
    }
    if (Apple *apple = dynamic_cast<Apple *>(bone)) {

        if (AppleBattle *appleb = dynamic_cast<AppleBattle *>(BattleScene::instance)) {
            appleb->nextLevelAction();
        }

        return true;
    }
    if (Target *target = dynamic_cast<Target *>(bone->getParent()->getParent())) {
        if (Hero *hero = dynamic_cast<Hero *>(bone->getParent()->getParent())) {
            if (hero->getPlayer()->getId() == _player_id)
                return false;
        }

        _disableArrow();

        addToNode(bone);

        return true;
    }
    return false;
}


void Arrow::_updateAnimation() {

}

void Arrow::update() {

}

void Arrow::addToNode(cocos2d::Node *bone) {
    if (Target *target = dynamic_cast<Target *>(bone->getParent()->getParent())) {

        const auto &position = this->getPosition();
        const auto &rotation = this->getRotation();
        _speedX /= 2;
        _speedY /= 2;
        if (rotation >= 180) {
            _speedY -= BattleScene::G;
        } else {
            _speedY += BattleScene::G;

        }
        this->setPosition(position.x + _speedX, position.y + _speedY);


        this->retain();
        this->removeFromParentAndCleanup(true);
        //position
        auto globalPoint = Variables::translatePoint(Vec3(0.f, 0.f, 0.f), _armatureDisplay,
                                                     bone);
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

        target->dealDamage(10.f);
        bone->addChild(this, -1);
        this->release();
    }
}

void Arrow::afterAction() {

}

PowerArrow::PowerArrow(const std::string &armatureName, float radian, float power,
                       const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                             player_id) {
}

bool PowerArrow::processContact(cocos2d::Node *bone) {

    if (bone == nullptr) {
        return false;
    }
    if (Apple *apple = dynamic_cast<Apple *>(bone->getParent())) {
        this->retain();
        this->removeFromParentAndCleanup(true);
        _armatureDisplay->setScale(1.2f);
        apple->getDisplay()->addChild(this, 5);
        apple->getDisplay()->getPhysicsBody()->setVelocity(Vec2(_speedX, _speedY));
        this->release();
        _disableArrow();
        return true;
    }
    if (Target *target = dynamic_cast<Target *>(bone->getParent()->getParent())) {
        auto display = target->getDisplay();
        display->retain();
        display->removeFromParentAndCleanup(true);
        display->setScale(0.9f);
        target->dealDamage(25.f);
        _armatureDisplay->addChild(display);
        display->release();
        return true;
    }
    return false;
}

PowerArrow::~PowerArrow() {

}

FrozenArrow::FrozenArrow(const std::string &armatureName, float radian, float power,
                         const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                               player_id) {

}

bool FrozenArrow::processContact(cocos2d::Node *bone) {
    return Arrow::processContact(bone);
}

FrozenArrow::~FrozenArrow() {

}

void FrozenArrow::update(float dt) {
    Arrow::update(dt);
}

FireArrow::FireArrow(const std::string &armatureName, float radian, float power,
                     const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                           player_id) {

}

bool FireArrow::processContact(cocos2d::Node *bone) {
    return Arrow::processContact(bone);
}

FireArrow::~FireArrow() {

}

void FireArrow::update(float dt) {
    Arrow::update(dt);
}

BombArrow::BombArrow(const std::string &armatureName, float radian, float power,
                     const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                           player_id) {

}

bool BombArrow::processContact(cocos2d::Node *bone) {

    _disableArrow();

    return false;
}


void BombArrow::afterAction() {
    ParticleExplosion *_emitter = ParticleExplosion::create();

    _emitter->retain();

    _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("stars.png"));

    _emitter->setAutoRemoveOnFinish(true);

    _emitter->setPosition(this->getPosition());

    _emitter->setSpeed(500.f);

    BattleScene::instance->addChild(_emitter);
}

void BombArrow::_disableArrow() {

    afterAction();

    this->retain();

    this->removeFromParent();
}


MineArrow::MineArrow(const std::string &armatureName, float radian, float power,
                     const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                           player_id) {

}

bool MineArrow::processContact(cocos2d::Node *bone) {
    return Arrow::processContact(bone);
}

MineArrow::~MineArrow() {

}

void MineArrow::update(float dt) {
    Arrow::update(dt);
}

void DuelArrow::update(float dt) {

    if (!_isActive) {
        return;
    }

    const auto &position = this->getPosition();
    const auto &rotation = this->getRotation();

    if (rotation >= 180) {
        _speedY -= BattleScene::G;
    } else {
        _speedY += BattleScene::G;

    }
    this->setPosition(position.x + _speedX, position.y + _speedY);
    this->setRotation(std::atan2(-_speedY, _speedX) * dragonBones::RADIAN_TO_ANGLE);


    DuelScene::instance->moveScene(_speedX);


    auto random = RandomHelper::random_real(0.f, 20.f);

    if (position.y < BattleScene::GROUND + random) {
        _disableArrow();
        if (DuelScene *duel = dynamic_cast<DuelScene *>(BattleScene::instance)) {
            duel->makeTurn(_player_id);
        }
    }

}

DuelArrow::DuelArrow(const std::string &armatureName, float radian, float power,
                     const cocos2d::Vec2 &position, int player_id) : Arrow(armatureName, radian, power, position,
                                                                           player_id) {
    if (DuelScene *duel = dynamic_cast<DuelScene *>(BattleScene::instance)) {
        duel->makeTurn(-1);
    }

}

bool DuelArrow::processContact(cocos2d::Node *bone) {
    if (Arrow::processContact(bone))
        if (DuelScene *duel = dynamic_cast<DuelScene *>(BattleScene::instance)) {
            duel->makeTurn(_player_id);
            return true;
        }
    return false;
}

DuelArrow::~DuelArrow() {

}
