#include <GameEngine/Objects/Environment/Apple.h>
#include <GameEngine/Global/WeaponSelector.h>
#include "AppleBattle.h"

USING_NS_CC;

void AppleBattle::initWorld() {

    GLOBAL_SCALE = 1.5f;


    Node *ground = Node::create();
    ground->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0.f, BattleScene::GROUND + origin.y),
                                                          Vec2(visibleSize.width, BattleScene::GROUND + origin.y)));
    ground->getPhysicsBody()->setContactTestBitmask(true);
    this->addChild(ground);

    initObjects();

    ui->initApple(visibleSize, _player);
}

void AppleBattle::initObjects() {

    _player = new Hero(50.f * this->GLOBAL_SCALE + origin.x, AppleBattle::GROUND + origin.y, new Player(1, "hero"));

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
                                _player->runAction(Spawn::createWithTwoActions(MoveTo::create(1.f, Vec2(50.f * GLOBAL_SCALE, AppleBattle::GROUND)),
                                                                               ScaleTo::create(1.f, _player->getNormalScale() * GLOBAL_SCALE)));
                            }
                    ),
                    CallFunc::create(
                            [&]() {

                                _targets[0]->runAction(Spawn::createWithTwoActions(MoveTo::create(1.f, Vec2(visibleSize.width - 100.f * GLOBAL_SCALE, AppleBattle::GROUND)),
                                                                                   ScaleTo::create(1.f, _targets[0]->getNormalScale() * GLOBAL_SCALE)));
                            }
                    )

            ),
            DelayTime::create(1),
            //TODO add spawn effect
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
    return false;
}

void AppleBattle::addApple() {
    auto apple = new Apple(_targets[0]->getPositionX(), _targets[0]->getGlobalHeight("Head"));
    _targets.push_back(apple);
}
