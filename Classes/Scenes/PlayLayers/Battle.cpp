#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Objects/WeaponSelector.h>
#include <GameEngine/Global/Producer.h>
#include <GameEngine/Global/Misc/UI/PopUp.h>


USING_NS_CC;

BattleScene *BattleScene::create(Statistics *stats) {
    BattleScene *ret = new(std::nothrow) BattleScene();
    if (ret && ret->init(stats)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    _instance = ret;

    return ret;
}

void BattleScene::removeBrain(Brain *brain) {
    if (!_brains.empty()) {
        brain->removeFromParent();
        _brains.erase(std::find(_brains.begin(), _brains.end(), brain));
    }
}

void BattleScene::addBrain(Brain *brain) {
    this->addChild(brain);
    _brains.push_back(brain);
}

void BattleScene::addTarget(cocos2d::Node *target) {
    _targets.push_back(target);
}

void BattleScene::removeTarget(cocos2d::Node *target) {
    if (!_targets.empty())
        _targets.erase(std::find(_targets.begin(), _targets.end(), target));
}

void BattleScene::initWorld() {

    _player = new DuelHero(50.f + origin.x, BattleScene::GROUND + origin.y + 200.f,
                           PlayerDuel::create(_client->getDBPlayer()->getId(),_client->getDBPlayer()->getName()));

    _createEnvForStickman(_player, 1);

    _ui->initBattle(visibleSize, _player);

//    Producer *prod = new Producer("cre");
//
//    prod->startLevel(1);
}


int BattleScene::getGainedCoinsByActionType(int type) {
    return 0;
}