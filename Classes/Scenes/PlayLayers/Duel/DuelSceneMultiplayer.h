#ifndef ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H
#define ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H


#include "DuelScene2P.h"
#include "Scenes/PlayLayers/MultiplayerBattle.h"

class DuelSceneMultiplayer : public MultiplayerBattle {
public:

    static DuelSceneMultiplayer *create(Statistics *stats);

    void makeTurn(int id) override;

    void setPlayer(int id) override;

protected:

    void initWorld() override;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) override;
};


#endif //ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H
