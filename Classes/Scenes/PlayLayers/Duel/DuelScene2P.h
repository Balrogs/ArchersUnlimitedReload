#ifndef ARCHERSUNLIMITED_DUELSCENE2P_H
#define ARCHERSUNLIMITED_DUELSCENE2P_H


#include "DuelScene.h"

class DuelScene2P : public DuelScene{
public:

    static DuelScene2P* create(Statistics* stats);

    void makeTurn(int id) override;

protected:

    void initWorld() override;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) override;

};

#endif //ARCHERSUNLIMITED_DUELSCENE2P_H
