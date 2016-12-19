#ifndef ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H
#define ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H


#include "DuelScene2P.h"

class DuelSceneMultiplayer : public DuelScene2P {
public:

    static DuelSceneMultiplayer* create(Statistics* stats);

    void makeTurn(int id) override;

    void createPlayers(Player* player1, Player* player2);

    void receiveAction(float angle, float power, int id);

    void setPlayer(int id);

    void startGame();

    void pauseGame();

    void resumeGame();

    void abort();

    void onPopScene() override;

protected:

    void onEnter();

    void initWorld() override;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) override;

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) override;


};


#endif //ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H
