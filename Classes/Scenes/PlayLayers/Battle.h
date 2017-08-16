#ifndef __BATTLE_H__
#define __BATTLE_H__

#include <GameEngine/Objects/Arrows/Arrow.h>
#include <GameEngine/Objects/Hero.h>
#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Objects/Aim/Aim.h>
#include <GameEngine/Brains/Brain.h>
#include <GameEngine/Global/Misc/SocketClient.h>
#include <GameEngine/Statistics.h>
#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "Scenes/Layers/UI.h"
#include "Scenes/Layers/BackgroundLayer.h"
#include "BattleParent.h"

class BattleScene : public BattleParent {
public:

    static BattleScene* create(Statistics* stats);

    void addBrain(Brain *brain);

    void removeBrain(Brain *brain);

    void addTarget(cocos2d::Node *target);

    void removeTarget(cocos2d::Node *target);

protected:

    std::vector<Brain *> _brains;

    std::vector<cocos2d::Node *> _targets;

    virtual void initWorld() override;

    virtual int _getGainedCoinsByActionType(int type) override;
};

#endif // __BATTLE_H__