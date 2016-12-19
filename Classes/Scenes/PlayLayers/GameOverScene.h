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
    void _saveStats(Statistics* stats);
};


#endif //ARCUNLIM_GAMEOVERSCENE_H
