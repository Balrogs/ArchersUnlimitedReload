
#include "Stickman.h"
#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/Variables.h>

USING_NS_CC;

Stickman::Stickman(float x_pos, float y_pos, float scale, float hp) : Body(x_pos, y_pos, scale, hp) {

    _armature = BattleScene::instance->factory.buildArmature("Stickman");
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();
    _armature->removeSlot(_armature->getSlot("Shoulders"));
    _armature->getAnimation().fadeIn(Variables::STICKMAN_SETUP_ANIMATION);
    for (auto child : _armatureDisplay->getChildren()) {
        auto bone = _armature->getBoneByDisplay(child);
        if (bone == nullptr) {
            continue;
        }
        if (bone->name == "Head") {
            child->setPhysicsBody(cocos2d::PhysicsBody::createCircle(child->getBoundingBox().size.height / 2,
                                                                     cocos2d::PHYSICSBODY_MATERIAL_DEFAULT));
        } else {
            child->setPhysicsBody(cocos2d::PhysicsBody::createBox(child->getBoundingBox().size,
                                                                  cocos2d::PHYSICSBODY_MATERIAL_DEFAULT));
        }
        auto physicsBody = child->getPhysicsBody();
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(0x01);
        physicsBody->setCollisionBitmask(0x02);
        physicsBody->setTag(2);
        physicsBody->setName(bone->name);
        physicsBody->setContactTestBitmask(true);
    }

    _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);
    this->setPosition(_x_pos, _y_pos);
    this->setScale(_normalScale * BattleScene::instance->getGlobalScale());
    _updateAnimation();


    dragonBones::WorldClock::clock.add(_armature);
    this->addChild(_armatureDisplay);
    BattleScene::instance->addChild(this, 2);
}

Stickman::~Stickman() {
}

void Stickman::update() {
    _updatePosition();
}

void Stickman::jump() {
    if (_state == JUMPING) {
        return;
    }

    _state = JUMPING;
    _speedY = _jump_speed;
    _armature->getAnimation().fadeIn(Variables::STICKMAN_SIT_ANIMATION);
}


void Stickman::sit() {
    if (_state == SITTING) {
        _armature->getAnimation().fadeIn(Variables::STICKMAN_UP_ANIMATION, -1.f, 1);
        _state = IDLE;
    } else {
        _speedY = -1.f;
        _armature->getAnimation().fadeIn(Variables::STICKMAN_SIT_ANIMATION, -1.f, 1);
        _state = SITTING;
    }

}


void Stickman::attack() {
    if (_state == ATTACKING) {
        return;
    }

    _state = ATTACKING;
    const auto animationName = "hit1";
    _armature->getAnimation().fadeIn(animationName);
}

void Stickman::aim() {
}

void Stickman::_updateAnimation() {
    if (_state == IDLE) {
        return;
    }
    if (_moveDir == 0) {
        _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);
    } else {
        _armature->getAnimation().fadeIn(Variables::STICKMAN_WALK_ANIMATION);
    }
}


Body::Body(float x_pos, float y_pos, float scale, float hp) :
        _faceDir(1),
        _moveDir(0),
        _speedX(0.f),
        _speedY(0.f),
        _state(IDLE) {
    _x_pos = x_pos;
    _y_pos = y_pos;
    _hp = hp;
    _normalScale = scale;
}

void Body::setSpeed(float move_speed) {
    _move_speed = move_speed;
}

void Body::setJumpSpeed(float jump_speed) {
    _jump_speed = jump_speed;
}

void Body::move(int dir) {

    if (_moveDir == dir) {
        return;
    }
    _state = MOVING;
    _moveDir = dir;
    _speedX = _move_speed * _moveDir;
    if (_moveDir) {
        if (_faceDir != _moveDir) {
            _faceDir = _moveDir;
            this->setScaleX(-this->getScaleX());
        }
    }

    _updateAnimation();
}

void Body::_updatePosition() {
    const auto &position = this->getPosition();
    if (_speedX != 0.f) {
        this->setPosition(position.x + _speedX, position.y);
        if (position.x < 20.f) {
            this->setPosition(20.f, position.y);
        }
    }

    if (_speedY != 0.f) {

        _speedY += BattleScene::G;

        this->setPosition(position.x, position.y + _speedY);
        if (position.y < BattleScene::GROUND) {
            this->setPosition(position.x, BattleScene::GROUND);
            _state = IDLE;
            _speedY = 0.f;
            _speedX = 0.f;
            _updateAnimation();
        }
    }
}

void Body::kill() {
    BattleScene::instance->removeTarget(this);
    dragonBones::WorldClock::clock.remove(_armature);
    BattleScene::instance->removeChild(this);
}

State Body::getState() {
    return _state;
}

void Body::setState(State state) {
    _state = state;
}
