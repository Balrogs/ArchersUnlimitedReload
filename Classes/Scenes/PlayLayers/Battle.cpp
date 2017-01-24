#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Global/WeaponSelector.h>
#include <GameEngine/Global/Producer.h>
#include <GameEngine/Global/Misc/PopUp.h>
#include <Scenes/Layers/BackgroundLayer.h>
#include <Localization/LocalizedStrings.h>
#include "Battle.h"
#include "Scenes/PlayLayers/Apple/AppleBattle.h"
#include "Scenes/PlayLayers/Duel/DuelScene.h"
#include "Scenes/PlayLayers/Duel/DuelScene2P.h"
#include "Scenes/PlayLayers/Duel/DuelSceneMultiplayer.h"
#include "GameOverScene.h"

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

    _player = new Hero(50.f + origin.x, BattleScene::GROUND + origin.y + 200.f);

    _createEnvForStickman(_player, 1);

    _ui->initBattle(visibleSize, _player);

//    Producer *prod = new Producer("cre");
//
//    prod->startLevel(1);
}


int BattleScene::_getGainedCoinsByActionType(int type) {
    return 0;
}