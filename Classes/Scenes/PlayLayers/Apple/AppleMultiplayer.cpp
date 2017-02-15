#include <GameEngine/Objects/Environment/Box.h>
#include "AppleMultiplayer.h"

AppleMultiplayer *AppleMultiplayer::create(Statistics *stats) {
    AppleMultiplayer *ret = new(std::nothrow) AppleMultiplayer();
    if (ret && ret->init(stats)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    _instance = ret;

    return ret;
}

void AppleMultiplayer::initWorld() {
    _isStarted = false;

    _client = SocketClient::getInstance();

    _GLOBAL_SCALE = 1.f;

    _isTargetHitted = false;

    _shotsLimit = 10;

    _completedShots = 0;

    initObjects();

}

bool AppleMultiplayer::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    if (_isStarted) {
        return BattleParent::_touchHandlerBegin(touch, event);
    } else
        return false;
}

void AppleMultiplayer::setPlayer(int id) {
    MultiplayerBattle::setPlayer(id);

    switch (id) {
        case 1: {
            _player = _hero1;
            break;
        }
        case 2: {
            _player = _hero2;
            break;
        }

        default:
            break;
    }

    //TODO
   // _ui->initApple(visibleSize, _hero1, _hero2);
}

void AppleMultiplayer::initObjects() {
    _env = Node::create();
    this->addChild(_env);

    _hero1 = new Hero(150.f * this->_GLOBAL_SCALE + origin.x, AppleBattle::GROUND );
    _hero2 = new Hero(visibleSize.width - 100.f * this->_GLOBAL_SCALE, AppleBattle::GROUND);

}

bool AppleMultiplayer::isGameOver() {
    if (_stats->getLevel() == 25)
        return true;

    if (_isAppleHitted) {
        _isAppleHitted = false;
        _stats->increaseLevel(_stats->getLevel() + 1);
        _nextLevelAction();
        return false;
    }

    return _completedShots - _shotsLimit == 0;
}

void AppleMultiplayer::_nextLevelAction() {

    _completedShots = 0;

    _env->removeAllChildren();

   // _ui->setWarning("SHOOT APPLE", Color3B::BLACK);

    switch (_stats->getLevel()) {
        case 0: {
            break;
        }
        case 1: {
            _GLOBAL_SCALE = 0.95f;
            break;
        }
        case 2: {
            _GLOBAL_SCALE = 0.9f;
            break;
        }
        case 3: {
            _GLOBAL_SCALE = 0.85f;
            break;
        }
        case 4: {
            _GLOBAL_SCALE = 0.8f;
            break;
        }
        case 5: {
            _GLOBAL_SCALE = 0.75f;
            break;
        }
        case 6: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(4);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            auto box2 = Box::create(4);
            box2->setPosition(Vec2(visibleSize.width / 2, GROUND + box->getBoundingBox().size.height));
            _env->addChild(box2);
            break;
        }
        case 7: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(4);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            auto box2 = Box::create(4);
            box2->setPosition(Vec2(visibleSize.width / 2, GROUND + box->getBoundingBox().size.height));
            _env->addChild(box2);
            auto box3 = Box::create(4);
            box3->setPosition(Vec2(visibleSize.width / 2, GROUND + box2->getBoundingBox().size.height));
            _env->addChild(box3);
            break;
        }
        case 8: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(3);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 9: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            auto box2 = Box::create(2);
            box2->setPosition(Vec2(visibleSize.width / 2 - box->getBoundingBox().size.width, GROUND));
            _env->addChild(box2);
            break;
        }
        case 10: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            auto box2 = Box::create(2);
            box2->setPosition(Vec2(visibleSize.width / 2 - box->getBoundingBox().size.width, GROUND));
            _env->addChild(box2);
            auto box3 = Box::create(4);
            box3->setPosition(Vec2(visibleSize.width / 2 - box2->getBoundingBox().size.width / 2,
                                   GROUND + box2->getBoundingBox().size.height));
            _env->addChild(box3);
            break;
        }
        case 11: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 12: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 13: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 14: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 15: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 16: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 17: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 18: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 19: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 20: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 21: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 22: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 23: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 24: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 25: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(2);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        default:
            return;
    }

    _bullet_pull->removeAllChildren();

    this->runAction(
            Sequence::create(
                    Spawn::createWithTwoActions(
                            CallFunc::create(
                                    [&]() {
                                        _hero1->runAction(Spawn::createWithTwoActions(
                                                MoveTo::create(1.f, Vec2(150.f * _GLOBAL_SCALE, AppleBattle::GROUND)),
                                                ScaleTo::create(1.f, _GLOBAL_SCALE)));
                                    }
                            ),
                            CallFunc::create(
                                    [&]() {

                                        _hero2->runAction(Spawn::createWithTwoActions(MoveTo::create(1.f,
                                                                                                          Vec2(visibleSize.width -
                                                                                                               100.f *
                                                                                                               _GLOBAL_SCALE,
                                                                                                               AppleBattle::GROUND)),
                                                                                           ScaleTo::create(1.f,
                                                                                                           -_GLOBAL_SCALE,
                                                                                                           _GLOBAL_SCALE)));
                                    }
                            )

                    ),
                    DelayTime::create(1),
                    CallFunc::create(
                            [&]() {
                                for(auto apple : _targets){
                                    apple->removeFromParent();
                                }
                                _targets.clear();
                                this->addApple(_hero1);
                                this->addApple(_hero2);
                            }
                    ),
                    NULL)

    );

}

void AppleMultiplayer::startGame() {
    _nextLevelAction();
    _startGame();
}


void AppleMultiplayer::completeShot() {
    if(_isAppleHitted){
        _completedShots++;
    }
}

void AppleMultiplayer::receiveAction(float angle, float power, int id) {
    if (id != _playerId) {
        getHero(_playerId)->attack(angle, power);
    }
}
