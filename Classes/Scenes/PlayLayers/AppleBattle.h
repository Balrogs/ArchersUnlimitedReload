#ifndef ARCUNLIM_APPLEBATTLE_H
#define ARCUNLIM_APPLEBATTLE_H

#include "cocos2d.h"
#include "Battle.h"

class AppleBattle : public BattleScene {

public:

    static AppleBattle* create(Statistics* stats);

    void setHit();

    void setAppleHit();

protected:

    virtual void _enterFrameHandler(float passedTime) override;

    cocos2d::Node* _env;

    void _nextLevelAction();

    void addApple();

    void initWorld() override;

    void initObjects();

    bool isGameOver() override;

    bool _isTargetHitted;

    bool _isAppleHitted;

    int _shotsLimit;
};


#endif //ARCUNLIM_APPLEBATTLE_H
