#include "Stickman.h"
#include <Scenes/PlayLayers/Battle.h>
#include <GameEngine/Global/Variables.h>

USING_NS_CC;

Stickman::Stickman(float x_pos, float y_pos, float scale, float hp): Body(x_pos, y_pos, scale, 1, PlayerView::randomBotView()) {
    _bot =  Bot::create("BOT", 100);
    _armature = BattleScene::getInstance()->factory.buildArmature("Stickman");
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();
    _armature->removeSlot(_armature->getSlot("Hands"));
    _armature->getAnimation().fadeIn(Variables::STICKMAN_SETUP_ANIMATION);
    for (auto child : _armatureDisplay->getChildren()) {
        auto bone = _armature->getBoneByDisplay(child);
        if (bone == nullptr) {
            continue;
        }
        CCLOG(bone->name.c_str());
        if (bone->name == "head") {
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
    _armature->removeSlot(_armature->getSlot("Apple"));

    _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);
    this->setPosition(_x_pos, _y_pos);
    this->setScale(BattleScene::getInstance()->getGlobalScale());
    _updateAnimation();

    _setPlayerView();

    dragonBones::WorldClock::clock.add(_armature);

    this->addChild(_armatureDisplay);

    BattleScene::getInstance()->addChild(this, 2);

    BattleScene::getInstance()->addStickman();
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
    switch (_state) {
        case HITTED:
            _armature->getAnimation().stop(_armature->getAnimation().getLastAnimationName());
            break;
        case IDLE:
            _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);
            break;
        case MOVING:
            _armature->getAnimation().fadeIn(Variables::STICKMAN_WALK_ANIMATION);
            break;
        case ATTACKING:
            _armature->getAnimation().fadeIn(Variables::STICKMAN_IDLE_ANIMATION);
            break;
        case JUMPING:
            _armature->getAnimation().fadeIn(Variables::STICKMAN_UP_ANIMATION);
            break;
        case SITTING:
            _armature->getAnimation().fadeIn(Variables::STICKMAN_SIT_ANIMATION);
            break;
    }
}

void Stickman::hit(cocos2d::Vec2 velocity) {

    setState(State::HITTED);

    this->runAction(Sequence::create(
            MoveBy::create(1.f, cocos2d::Vec3(velocity.x, 0.f, 0.f)),
            RotateTo::create(0.5f, 0.f),
            CallFunc::create(
                    [&]() {
                        setState(IDLE);
                    }
            ),
            NULL)
    );
}

bool Stickman::getHP() {
    return _bot->getHp() > 0;
}

void Stickman::dealDamage(float d, Node* bone) {
    auto name = bone->getPhysicsBody()->getName();
    float factor = getBoneFactor(name);
    _bot->setHp((int)(d * factor));
}

Player *Stickman::getPlayer() {
    return _bot;
}

void Stickman::_changeArrow() {

}

void Stickman::_changeHat() {
    _hat = BattleParent::getInstance()->factory.buildArmature(_playerView->getHat()->Path());
    _armature->getSlot("Hat")->setChildArmature(_hat);
}

void Stickman::_changeBow() {

}

Body::Body(float x_pos, float y_pos, float scale, int facedir, PlayerView* playerView) :
        _faceDir(facedir),
        _moveDir(0),
        _speedX(0.f),
        _speedY(0.f),
        _state(IDLE),
        _playerView(playerView) {
    _x_pos = x_pos;
    _y_pos = y_pos;
}

void Body::setSpeed(float move_speed) {
    _move_speed = move_speed;
}

void Body::setJumpSpeed(float jump_speed) {
    _jump_speed = jump_speed;
}

void Body::move(int dir) {

    setState(MOVING);

    if (_moveDir == dir) {
        return;
    }

    _moveDir = dir;
    _speedX = _move_speed * _moveDir;
    if (_moveDir) {
        if (_faceDir != _moveDir) {
            changeFacedir(_moveDir);
        }
    } else {
        setState(IDLE);
    }
}

void Body::changeFacedir(int facedir) {
    _faceDir = facedir;
    if(getScaleX() > 0)
        this->setScaleX(facedir * this->getScaleX());
    else
        this->setScaleX(-facedir * this->getScaleX());
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
        if (position.y < BattleScene::getInstance()->GROUND) {
            this->setPosition(position.x, BattleScene::getInstance()->GROUND);
            _speedY = 0.f;
            _speedX = 0.f;
        }
    }
}

void Body::kill() {
    if(BattleScene* battleScene = dynamic_cast<BattleScene*>(BattleParent::getInstance())){
        battleScene->removeTarget(this);
    }
    dragonBones::WorldClock::clock.remove(_armature);
    BattleParent::getInstance()->removeChild(this);
}

State Body::getState() {
    return _state;
}

void Body::setState(State state) {
    if (_state != state) {
        _state = state;
        _updateAnimation();
    }
}

void Body::_setPlayerView() {
    _changeArrow();
    _changeHat();
    _changeBow();
}
void Body::setPlayerView(PlayerView* playerView) {
    _playerView = playerView;

    _setPlayerView();
}

float Body::getBoneFactor(std::string name) {
    if(name == "head"){
        return 1.5f;
    } else if(name == "body") {
        return 1.f;
    } else if(name == "left arm" || name == "right arm"){
        return 0.75f;
    } else if(name == "left leg" || name == "right leg"){
        return 0.75f;
    }  else if(name == "left a" || name == "right a"){
        return .5f;
    } else if(name == "left l" || name == "right l"){
        return .5f;
    } else {
        return 0.f;
    }
}


