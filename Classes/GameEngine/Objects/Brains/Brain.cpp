//
// Created by igor on 08.09.16.
//

#include <Scenes/PlayLayers/Battle.h>

USING_NS_CC;

Brain::Brain(Body *body) : Brain(body, 20.f, 2.f, 0.f) {

}

Brain::Brain(Body *body, float jump_speed, float move_speed, float upd) :
        _move_speed(move_speed),
        _jump_speed(jump_speed),
        _current_dir(-1) {
    _body = body;
    _body->setSpeed(_move_speed);
    _body->setJumpSpeed(_jump_speed);
    this->schedule(cocos2d::SEL_SCHEDULE(&Brain::update), upd);
}

Brain::~Brain() {

}

void Brain::update(float dt) {
    update();
}

void Brain::update() {

    if (!_body->getHP()) {
        remove();
        return;
    }
    auto state = _body->getState();
    switch (state) {
        case MOVING: {
            if (isTargetNear()) {
                _body->move(0);
                _body->attack();
            } else {
                _body->move(chooseDir());
            }
            break;
        }
        case ATTACKING: {
            if (isTargetNear()) {
                _body->attack();
            } else {
                _body->move(chooseDir());
            }
            break;
        }
        case JUMPING: {

            break;
        }
        case HITTED : {
            return;
        }
        case SITTING: {

            break;
        }
        case IDLE: {
            _body->move(chooseDir());
            break;
        }
    }
    _body->update();
}

int Brain::chooseDir() {
    auto position = _body->getPosition();
    if (position.x <= 20.f) {
        _current_dir = 1;
    } else if (position.x >= cocos2d::Director::getInstance()->getVisibleSize().width - 20.f) {
        _current_dir = -1;
    }
    return _current_dir;
}

bool Brain::isTargetNear() {
    return false;
}


void Brain::remove() {
    if(BattleScene* battleScene = dynamic_cast<BattleScene*>(BattleParent::getInstance())){
        battleScene->removeBrain(this);
    }

    _body->kill();
}


PassiveBrain::PassiveBrain(Body *body) : Brain(body, 0.f, 0.f, 0.f) {
}

void PassiveBrain::update() {
    _body->update();
    if (!_body->getHP()) {
        remove();
        return;
    }
}


FastBrain::FastBrain(Body *body) : Brain(body, 25.f, 4.f, 0.f) {

}


void FastBrain::update() {
    Brain::update();
}

StrengthBrain::StrengthBrain(Body *body) : Brain(body, 15.f, 1.2f, 0.f) {

}

void StrengthBrain::update() {
    Brain::update();
}

HeroBrain::HeroBrain(Body *body, float upd) : Brain(body, 20.f, 2.f, upd) {
    this->schedule(cocos2d::SEL_SCHEDULE(&Brain::update), upd);
}

void HeroBrain::update() {

    if (!_body->getHP()) {
        remove();
        return;
    }
    auto state = _body->getState();
    switch (state) {
        case MOVING: {
            if (isTargetNear()) {
                _body->move(0);
            } else {
                _body->move(chooseDir());
            }
            break;
        }
        case ATTACKING:
            if (!isTargetNear()) {
                _body->move(chooseDir());
            }
            break;
        case IDLE: {
            if (isTargetNear()) {
                DelayTime *pDelayTime = DelayTime::create(1.f);
                CallFunc *addtarg = CallFunc::create(CC_CALLBACK_0(HeroBrain::attack, this));
                CallFunc *aimtarg = CallFunc::create(CC_CALLBACK_0(HeroBrain::aim, this));
                _body->setState(ATTACKING);
                this->runAction(RepeatForever::create(Sequence::create(aimtarg, pDelayTime, addtarg, NULL)));
            } else {
                _body->move(chooseDir());
            }
            break;
        }
        default:
            break;
    }
    _body->update();
}

bool HeroBrain::isTargetNear() {
    auto position = _body->getPosition();
    return position.x <= cocos2d::Director::getInstance()->getVisibleSize().width - 100;
}

void HeroBrain::aim() {
    _body->aim();
}

void HeroBrain::attack() {
    _body->attack();
}

HeroBrainDuel::HeroBrainDuel(Hero *body, float upd) : Brain(body, 20.f, 2.f, upd) {
}

bool HeroBrainDuel::isTargetNear() {
    return true;
}

void HeroBrainDuel::update() {
    if (!_body->getHP()) {
        remove();
        return;
    }
    auto state = _body->getState();
    switch (state) {
        case IDLE: {
            _body->setState(ATTACKING);
            _body->runAction(Sequence::create(
                    CallFunc::create([&]() {
                        _body->aim();
                    }),
                    DelayTime::create(1.f),
                    CallFunc::create([&]() {
                        _body->attack();
                    }),
                    NULL)

            );
            break;
        }
        default:
            break;
    }
    _body->update();

}

void HeroBrainDuel::attack() {
    _body->attack();
}

void HeroBrainDuel::aim() {
    _body->aim();
}
