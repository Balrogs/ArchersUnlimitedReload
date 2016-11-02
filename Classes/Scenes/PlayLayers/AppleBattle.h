//
// Created by igor on 22.09.16.
//

#ifndef ARCUNLIM_APPLEBATTLE_H
#define ARCUNLIM_APPLEBATTLE_H

#include "cocos2d.h"
#include "Battle.h"

class AppleBattle : public BattleScene {

public:
    CREATE_FUNC(AppleBattle);

    void nextLevelAction();

    void setHit();

protected:

    void addApple();

    void initWorld() override;

    void initObjects();

    bool isGameOver() override;

    bool _isTargetHitted;
};


#endif //ARCUNLIM_APPLEBATTLE_H
