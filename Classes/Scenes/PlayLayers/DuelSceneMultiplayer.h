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


protected:

    void initWorld() override;

    bool _touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) override;



};


#endif //ARCHERSUNLIMITED_DUELSCENEMULTIPLAYER_H
