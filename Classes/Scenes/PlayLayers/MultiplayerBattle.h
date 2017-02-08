#ifndef ARCHERSUNLIMITED_MULTIPLAYERBATTLE_H
#define ARCHERSUNLIMITED_MULTIPLAYERBATTLE_H


#include <GameEngine/Player.h>
#include "Scenes/PlayLayers/Duel/DuelScene2P.h"

class MultiplayerBattle : public DuelScene2P {
public:
    virtual void createPlayers(Player* player1, Player* player2);

    virtual void receiveAction(float angle, float power, int id);

    virtual void setPlayer(int id);

    virtual void startGame();

    virtual void pauseGame();

    virtual void resumeGame();

    virtual void abort();

    virtual void onPopScene();

    int getPlayerId() override;

protected:

    int _playerId;

    void onEnter();

    bool _touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) override;
};


#endif //ARCHERSUNLIMITED_MULTIPLAYERBATTLE_H
