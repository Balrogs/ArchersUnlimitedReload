#ifndef ARCHERSUNLIMITED_APPLEMULTIPLAYER_H
#define ARCHERSUNLIMITED_APPLEMULTIPLAYER_H


#include "AppleBattle.h"
#include "Scenes/PlayLayers/MultiplayerBattle.h"

class AppleMultiplayer: public AppleParent, public MultiplayerBattle {
public:

    static AppleMultiplayer* create(Statistics* stats);

    void _nextLevelAction() override;

    void addApple() override;

    void setPlayer(int id) override;

    void makeTurn(int id);

protected:

    void initWorld() override;

    void initObjects();

    bool isGameOver() override;

    void setTurnId(int id);

    bool _touchHandlerBegin(const Touch *touch, Event *event) override;
};


#endif //ARCHERSUNLIMITED_APPLEMULTIPLAYER_H
