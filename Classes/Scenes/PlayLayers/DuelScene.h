//
// Created by igor on 22.09.16.
//

#ifndef ARCUNLIM_DUELSCENE_H
#define ARCUNLIM_DUELSCENE_H

#include <GameEngine/Player.h>
#include "cocos2d.h"
#include "Battle.h"


class DuelScene : public BattleScene {
public:
    CREATE_FUNC(DuelScene);

    void makeTurn(int id);

protected:
    void initWorld() override;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) override;
    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) override;
    bool isGameOver() override;

private:
    int _turnId;
    Player *_player1;
    Player *_player2;
};


#endif //ARCUNLIM_DUELSCENE_H
