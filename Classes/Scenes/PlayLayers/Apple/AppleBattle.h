#ifndef ARCUNLIM_APPLEBATTLE_H
#define ARCUNLIM_APPLEBATTLE_H

#include "cocos2d.h"
#include "Scenes/PlayLayers/BattleParent.h"
#include "AppleParent.h"


class AppleBattle : public BattleParent, public AppleParent{
public:

    static AppleBattle* create(Statistics* stats);

    void _nextLevelAction() override;

    void addApple() override;

    void completeShot() override;

protected:

    virtual int _getGainedCoinsByActionType(int type) override ;

    cocos2d::Node* _env;

    void initWorld() override;

    void initObjects();

    bool isGameOver() override;

};


#endif //ARCUNLIM_APPLEBATTLE_H