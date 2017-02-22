#ifndef ARCHERSUNLIMITED_MULTIPLAYERBATTLE_H
#define ARCHERSUNLIMITED_MULTIPLAYERBATTLE_H


#include <GameEngine/Player.h>
#include "Scenes/PlayLayers/Duel/DuelScene2P.h"

class MultiplayerBattle : public DuelScene2P {
public:
    virtual void createPlayers(Player* player1, Player* player2);

    virtual void receiveAction(float angle, float power, int id, int x, int y);

    virtual void receiveMove(int dir, int id);

    virtual void receiveAim(float angle, float power, int id);

    virtual void setPlayer(int id);

    virtual void startGame();

    virtual void pauseGame();

    virtual void resumeGame();

    virtual void abort();

    virtual void onPopScene();

    virtual void movePlayer(int dir);

    int getPlayerId() override;

    virtual void waitForPlayer();

protected:

    int _playerId;

    void onEnter();

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) override;

    bool _touchHandlerMove(const Touch *touch, Event *event) override;
};


#endif //ARCHERSUNLIMITED_MULTIPLAYERBATTLE_H
