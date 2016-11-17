//
// Created by igor on 17.11.16.
//

#ifndef ARCHERSUNLIMITED_DUELSCENE2P_H
#define ARCHERSUNLIMITED_DUELSCENE2P_H


#include "DuelScene.h"

class DuelScene2P : public DuelScene{
public:
    CREATE_FUNC(DuelScene2P);

    void makeTurn(int id) override;

    Hero* getHero(int id);

protected:

    void initWorld() override;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) override;

    Hero* _hero1;
    Hero* _hero2;
};

#endif //ARCHERSUNLIMITED_DUELSCENE2P_H
