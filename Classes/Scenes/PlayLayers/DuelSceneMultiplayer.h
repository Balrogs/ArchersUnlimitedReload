//
// Created by igor on 17.11.16.
//

#ifndef ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H
#define ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H


#include "DuelScene2P.h"

class DuelSceneMultiplayer : public DuelScene2P {
public:
    CREATE_FUNC(DuelSceneMultiplayer);

    void makeTurn(int id) override;

    void createPlayers(Player* player1, Player* player2);

    void receiveAction(float angle, float power);

    void setPlayer(int id);

    void startGame();

    void pauseGame();

    void resumeGame();

    void abort();

protected:

    void onEnter();

    bool _isStarted;

    bool _isGamePaused;

    SocketClient* _client;

    void initWorld() override;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) override;

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) override;

};


#endif //ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H
