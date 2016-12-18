//
// Created by igor on 05.10.16.
//

#ifndef ARCUNLIM_GAMEOVERSCENE_H
#define ARCUNLIM_GAMEOVERSCENE_H

#include <GameEngine/Statistics.h>
#include "cocos2d.h"

USING_NS_CC;

class GameOverScene : public Layer {
public:

    static GameOverScene *create(Statistics* stats);

    bool init(Statistics* stats);

protected:
    Label* _title1;
    Label* _title2;
    void onQuit();
    void update(float dt);
};


#endif //ARCUNLIM_GAMEOVERSCENE_H
