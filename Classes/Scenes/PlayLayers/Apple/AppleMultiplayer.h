#ifndef ARCHERSUNLIMITED_APPLEMULTIPLAYER_H
#define ARCHERSUNLIMITED_APPLEMULTIPLAYER_H


#include "AppleBattle.h"
#include "Scenes/PlayLayers/MultiplayerBattle.h"

class AppleMultiplayer: public AppleParent, public MultiplayerBattle {
public:

    static AppleMultiplayer* create(Statistics* stats);

    void _nextLevelAction() override;

    void setPlayer(int id) override;

    void completeShot() override;

    void startGame() override;

    void receiveAction(float angle, float power, int id, int x, int y) override;
    void receiveAim(float angle, float power, int id) override;

protected:

    bool _isWaiting = false;

    float _penalty = 3.f;

    void initWorld() override;

    void initObjects();

    bool isGameOver() override;

    bool _touchHandlerBegin(const Touch *touch, Event *event) override;
};


#endif //ARCHERSUNLIMITED_APPLEMULTIPLAYER_H
