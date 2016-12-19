#ifndef ARCUNLIM_DUELSCENE_H
#define ARCUNLIM_DUELSCENE_H

#include <GameEngine/Player.h>
#include "Battle.h"


class DuelScene : public BattleScene {
public:

    static DuelScene* create(Statistics* stats);

    virtual void makeTurn(int id);

protected:
    int _turnId;

    int _getGainedCoinsByActionType(int type) override;

    Player *_player1;
    Player *_player2;

    Hero* _hero1;
    Hero* _hero2;

    bool _isStarted;

    SocketClient* _client;

    void initWorld() override;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) override;

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) override;

    bool isGameOver() override;

    void _startGame();

};


#endif //ARCUNLIM_DUELSCENE_H
