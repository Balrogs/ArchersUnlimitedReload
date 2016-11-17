#include <GameEngine/Objects/Environment/Apple.h>
#include <GameEngine/Global/WeaponSelector.h>
#include <GameEngine/Objects/Environment/Ground.h>
#include "AppleBattle.h"

USING_NS_CC;

void AppleBattle::initWorld() {

    GLOBAL_SCALE = 1.f;

    _isTargetHitted = false;

    Ground *ground = new Ground(GROUND, visibleSize.width);

    this->addChild(ground);

    initObjects();

    ui->initApple(visibleSize, _player);
}

void AppleBattle::initObjects() {

    _player = new AppleHero(50.f * this->GLOBAL_SCALE + origin.x, AppleBattle::GROUND, "HERO");

    auto target = new Stickman(visibleSize.width - 100.f * this->GLOBAL_SCALE, AppleBattle::GROUND, 0.3f, 10);
    auto apple = new Apple(target->getPositionX(), target->getGlobalHeight("Head"));

    _targets.push_back(target);
    _targets.push_back(apple);
    _brains.push_back(new PassiveBrain(target));
}


void AppleBattle::nextLevelAction() {
    GLOBAL_SCALE -= 0.05f;

    _bullet_pull->removeAllChildren();

    this->runAction(
            Sequence::create(
                    Spawn::createWithTwoActions(
                            CallFunc::create(
                                    [&]() {
                                        _player->runAction(Spawn::createWithTwoActions(
                                                MoveTo::create(1.f, Vec2(50.f * GLOBAL_SCALE, AppleBattle::GROUND)),
                                                ScaleTo::create(1.f, GLOBAL_SCALE)));
                                    }
                            ),
                            CallFunc::create(
                                    [&]() {

                                        _targets[0]->runAction(Spawn::createWithTwoActions(MoveTo::create(1.f,
                                                                                                          Vec2(visibleSize.width -
                                                                                                               100.f *
                                                                                                               GLOBAL_SCALE,
                                                                                                               AppleBattle::GROUND)),
                                                                                           ScaleTo::create(1.f,
                                                                                                           GLOBAL_SCALE)));
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
    return _isTargetHitted;
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
