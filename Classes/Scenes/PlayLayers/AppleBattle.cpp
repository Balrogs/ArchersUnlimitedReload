#include <GameEngine/Objects/Environment/Apple.h>
#include <GameEngine/Global/WeaponSelector.h>
#include <GameEngine/Objects/Environment/Box.h>
#include "AppleBattle.h"

USING_NS_CC;

AppleBattle *AppleBattle::create(Statistics *stats) {
    AppleBattle *ret = new(std::nothrow) AppleBattle();
    if (ret && ret->init(stats)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AppleBattle::initWorld() {

    _GLOBAL_SCALE = 1.f;

    _isTargetHitted = false;

    _shotsLimit = 5;

    initObjects();

    _ui->initApple(visibleSize, _player);

    _nextLevelAction();
}

void AppleBattle::initObjects() {

    _env = Node::create();
    this->addChild(_env);
    _player = new AppleHero(50.f * this->_GLOBAL_SCALE + origin.x, AppleBattle::GROUND, "HERO");

    auto target = new Stickman(visibleSize.width - 100.f * this->_GLOBAL_SCALE, AppleBattle::GROUND, 0.3f, 10);
    target->changeFacedir(-1);
    auto apple = new Apple(target->getPositionX(), target->getGlobalHeight("Head"));

    _targets.push_back(target);
    _targets.push_back(apple);
    _brains.push_back(new PassiveBrain(target));
}


void AppleBattle::_nextLevelAction() {
    _player->getPlayer()->nullShotsCount();
    _env->removeAllChildren();
    _ui->setWarning("SHOOT APPLE", Color3B::BLACK);

    switch (_stats->getLevel()) {
        case 0: {
            return;
        }
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: {
            _GLOBAL_SCALE -= 0.05f;
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
                                        _player->runAction(Spawn::createWithTwoActions(
                                                MoveTo::create(1.f, Vec2(50.f * _GLOBAL_SCALE, AppleBattle::GROUND)),
                                                ScaleTo::create(1.f, _GLOBAL_SCALE)));
                                    }
                            ),
                            CallFunc::create(
                                    [&]() {

                                        _targets[0]->runAction(Spawn::createWithTwoActions(MoveTo::create(1.f,
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
                                auto apple = _targets[1];
                                this->removeTarget(apple);
                                this->removeChild(apple);
                                this->addApple();
                            }
                    ),
                    NULL)

    );

}

bool AppleBattle::isGameOver() {
    if (_stats->getLevel() == 25)
        return true;

    if (_isAppleHitted) {
        _isAppleHitted = false;
        _stats->increaseLevel(_stats->getLevel() + 1);
        _nextLevelAction();
        return false;
    }

    return _isTargetHitted || _player->getPlayer()->getShotsCount() - _shotsLimit == 0;
}

void AppleBattle::addApple() {
    if (DragonObject *target = dynamic_cast<DragonObject *>(_targets[0])) {
        auto apple = new Apple(_targets[0]->getPositionX(), target->getGlobalHeight("Head"));
        _targets.push_back(apple);
    }
}

void AppleBattle::setHit() {
    _isTargetHitted = true;
}

void AppleBattle::_enterFrameHandler(float passedTime) {
    BattleScene::_enterFrameHandler(passedTime);
    auto shotsLeft = _shotsLimit - _player->getPlayer()->getShotsCount();
    if (shotsLeft == 1) {
        _ui->setWarning(StringUtils::format("%d SHOT LEFT", shotsLeft).c_str(), Color3B::RED);
    } else if (shotsLeft <= 2) {
        _ui->setWarning(StringUtils::format("%d SHOTS LEFT", shotsLeft).c_str(), Color3B::RED);
    }
}

void AppleBattle::setAppleHit() {
    _isAppleHitted = true;
}