#include <GameEngine/Objects/Environment/Apple.h>
#include <GameEngine/Objects/Environment/Box.h>
#include <GameEngine/Brains/Brain.h>
#include "AppleBattle.h"

USING_NS_CC;

AppleBattle *AppleBattle::create(Statistics *stats) {
    AppleBattle *ret = new(std::nothrow) AppleBattle();
    if (ret && ret->init(stats)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    _instance = ret;

    return ret;
}

void AppleBattle::initWorld() {

    _GLOBAL_SCALE = 1.f;

    _isTargetHitted = false;

    _shotsLimit = 5;

    _completedShots = 0;

    initObjects();

    _ui->initApple(visibleSize, _player);

    _nextLevelAction();
}

void AppleBattle::initObjects() {

    _env = Node::create();
    this->addChild(_env);

    _player = new AppleHero(150.f * this->_GLOBAL_SCALE + origin.x, AppleBattle::GROUND,
                            PlayerApple::create(_client->getDBPlayer()->getId(),  _client->getDBPlayer()->getName(), _stats->getCoinsGained()));

    auto target = new Stickman(visibleSize.width - 100.f * this->_GLOBAL_SCALE, AppleBattle::GROUND, 0.3f, 10);
    target->changeFacedir(-1);
    target->setPlayerView(PlayerView::defaultView());

    _targets.push_back(target);
    new PassiveBrain(target);

    addApple(target);
}


bool AppleBattle::isGameOver() {
    if (_stats->getLevel() == 25)
        return true;

    if (_isAppleHitted) {
        _isAppleHitted = false;
        _stats->increaseLevel(11);
        _nextLevelAction();
        return false;
    }

    return _isTargetHitted || _completedShots - _shotsLimit == 0;
}



int AppleBattle::getGainedCoinsByActionType(int type) {
    int value = 0;
    switch (type) {
        case 1: {
            auto shotsLeft = _shotsLimit - _completedShots;
            value = 100 + 10 * _stats->getLevel() - 5 * shotsLeft;
            break;
        }
        default: {
            break;
        }
    }
    return value;
}


void AppleBattle::_nextLevelAction() {
    _completedShots = 0;

    _env->removeAllChildren();

    _ui->setWarning("SHOOT APPLE", Color3B::BLACK);

    switch (_stats->getLevel()) {
        case 0: {
            return;
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
            //
            break;
        }
        case 5: {
            //
            break;
        }
        case 6: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::WOODEN_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            auto box2 = Box::create(BoxType::WOODEN_BOX);
            box2->setPosition(Vec2(visibleSize.width / 2, GROUND + box->getBoundingBox().size.height));
            _env->addChild(box2);
            break;
        }
        case 7: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::WOODEN_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            auto box2 = Box::create(BoxType::WOODEN_BOX);
            box2->setPosition(Vec2(visibleSize.width / 2, GROUND + box->getBoundingBox().size.height));
            _env->addChild(box2);
            auto box3 = Box::create(BoxType::WOODEN_BOX);
            box3->setPosition(Vec2(visibleSize.width / 2, GROUND + box2->getBoundingBox().size.height));
            _env->addChild(box3);
            break;
        }
        case 8: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_LARGE_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 9: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            auto box2 = Box::create(BoxType::STONE_MEDIUM_BOX);
            box2->setPosition(Vec2(visibleSize.width / 2 - box->getBoundingBox().size.width, GROUND));
            _env->addChild(box2);
            break;
        }
        case 10: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            auto box2 = Box::create(BoxType::STONE_MEDIUM_BOX);
            box2->setPosition(Vec2(visibleSize.width / 2 - box->getBoundingBox().size.width, GROUND));
            _env->addChild(box2);
            auto box3 = Box::create(BoxType::WOODEN_BOX);
            box3->setPosition(Vec2(visibleSize.width / 2 - box2->getBoundingBox().size.width / 2,
                                   GROUND + box2->getBoundingBox().size.height));
            _env->addChild(box3);
            break;
        }
        case 11: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_LARGE_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            auto box2 = Box::create(BoxType::WOODEN_BOX);
            box2->setPosition(Vec2(visibleSize.width / 2, GROUND + box->getBoundingBox().size.height));
            _env->addChild(box2);
            break;
        }
        case 12: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 13: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 14: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 15: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 16: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 17: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 18: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 19: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 20: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 21: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 22: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 23: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 24: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 25: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 26: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 27: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 28: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 29: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 30: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 31: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 32: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 33: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 34: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 35: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 36: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 37: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 38: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 39: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 40: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 41: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 42: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 43: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 44: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 45: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 46: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 47: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 48: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 49: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
            box->setPosition(Vec2(visibleSize.width / 2, GROUND));
            _env->addChild(box);
            break;
        }
        case 50: {
            _GLOBAL_SCALE = 1.f;
            auto box = Box::create(BoxType::STONE_MEDIUM_BOX);
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
                                                MoveTo::create(1.f, Vec2(150.f * _GLOBAL_SCALE, AppleBattle::GROUND)),
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
                                if (Body *target = dynamic_cast<Body *>(_targets[0])) {
                                    this->addApple(target);
                                }
                            }
                    ),
                    NULL)

    );

}

//TODO Add localization
void AppleBattle::completeShot() {
    AppleParent::completeShot();

    auto shotsLeft = _shotsLimit - _completedShots;
    if (shotsLeft == 1) {
        _ui->setWarning(StringUtils::format("%d SHOT LEFT", shotsLeft).c_str(), Color3B::RED);
    } else if (shotsLeft <= 2) {
        _ui->setWarning(StringUtils::format("%d SHOTS LEFT", shotsLeft).c_str(), Color3B::RED);
    } else {
        _ui->setWarning(StringUtils::format("%d SHOTS LEFT", shotsLeft).c_str(), Color3B::BLACK);
    }
}
